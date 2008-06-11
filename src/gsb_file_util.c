/* file gsb_file_util.c
 * used to save the gsb files */
/*     Copyright (C)	2000-2005 Cédric Auger (cedric@grisbi.org) */
/* 			http://www.grisbi.org */

/*     This program is free software; you can redistribute it and/or modify */
/*     it under the terms of the GNU General Public License as published by */
/*     the Free Software Foundation; either version 2 of the License, or */
/*     (at your option) any later version. */

/*     This program is distributed in the hope that it will be useful, */
/*     but WITHOUT ANY WARRANTY; without even the implied warranty of */
/*     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the */
/*     GNU General Public License for more details. */

/*     You should have received a copy of the GNU General Public License */
/*     along with this program; if not, write to the Free Software */
/*     Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA */


#include "include.h"

#include <zlib.h>


/*START_INCLUDE*/
#include "gsb_file_util.h"
#include "./dialog.h"
#include "./gsb_data_account.h"
#include "./gsb_data_transaction.h"
#include "./utils_str.h"
#include "./utils_files.h"
#include "./include.h"
#include "./erreur.h"
#include "./structures.h"
/*END_INCLUDE*/

/*START_STATIC*/
/*END_STATIC*/


/*START_EXTERN*/
extern gchar *nom_fichier_comptes ;
/*END_EXTERN*/


/**
 * check if the file exists, and if so, ask the user if he wants
 * to overwrite it
 * show an error message if necessary
 * return TRUE if the saving action can continue, FALSE to stop it
 *
 * \param filename
 *
 * \return TRUE ok, can continue (no file exists or user wants to overwrite), FALSE stop the action
 * */
gboolean gsb_file_util_test_overwrite ( const gchar *filename )
{
    if (!filename
	||
	!strlen (filename))
    {
	dialogue_error (_("No name to the file !"));
	return FALSE;
    }

    if (g_file_test ( filename,
		      G_FILE_TEST_EXISTS ))
    {
	/* the file exists */
	if (g_file_test (filename,
			 G_FILE_TEST_IS_DIR))
	{
	    gchar* tmpstr = g_strdup_printf ( _("%s is a directory...\nPlease choose another name."),
					       filename );
	    dialogue_error ( tmpstr );
	    g_free ( tmpstr );
	    return FALSE;
	}
	
	gchar* tmpstr = g_strdup_printf (_("Do you want to overwrite file \"%s\"?"), filename);
	gboolean response = question_yes_no_hint (_("File already exists"),
				       tmpstr,
				       GTK_RESPONSE_NO );
	g_free ( tmpstr );
        return response;
    }
    return TRUE;
}


/**
 * this function do the same as g_file_get_contents
 * but can also open a compressed file with zlib
 *
 * \param filename the name of file to open
 * \param file_content a gchar pointer to fill with the adress of the content of the file
 * \param length a gulong pointer to file with the length of the loaded file
 *
 * \return TRUE all is ok, FALSE a problem occured
 * */
gboolean gsb_file_util_get_contents ( gchar *filename,
				      gchar **file_content,
				      gulong *length )
{
    gzFile file;
    struct stat stat_buf;
    gulong alloc_size;
    gulong orig_size;
    gchar *content;
    gulong iterator = 0;
    gulong bytes_read;
    gboolean eof = 0;

    file = gzopen (filename, "rb");
    if (!file)
	return FALSE;

    /* FIXME windows and stat ??? */
    if (stat (filename, &stat_buf))
    {
	gchar *tmpstr = g_strdup_printf ( _("Grisbi cannot stat file %s, please check the file."),
					  filename);
	dialogue_error (tmpstr);
	g_free (tmpstr);
	return FALSE;
    }

    orig_size = stat_buf.st_size;

    if (gzdirect (file))
	/* the file is not compressed, keep the original size */
	alloc_size = orig_size + 1;
    else
	/* the file is compressed, the final size should be about 20x more
	 * FIXME this is not completly true, if the file is compressed AND crypted,
	 * the size doesn't really change. but i can't know here if the file is crypted
	 * because encrypt before compressed. perhaps forbid to compress a crypted file ?
	 * but i don't think it's a big thing because that allocated memory will be freed
	 * after loading the file ...*/
	alloc_size = 20 * orig_size;

    content = g_malloc0 (alloc_size);
    if (!content)
    {
	dialogue_error_memory ();
	return FALSE;
    }

    bytes_read = 0;

    /* we should be able to get directly the orig_size
     * for most of files it's enough, if the file is compressed,
     * we continue */
    iterator = gzread (file, content, orig_size);

    if (iterator < 0)
    {
	int save_errno = errno;
	gchar *tmpstr;

	g_free (content);
	tmpstr = g_strdup_printf ( _("Failed to read from file '%s': %s"),
				   filename, g_strerror (save_errno));
	dialogue_error (tmpstr);
	g_free (tmpstr);
	return FALSE;
    }

    /* ok, now add caracter by caracter untill the end of the file */
    do
    {
	gchar c;

	c = gzgetc (file);

	eof = gzeof (file);
	if (!eof)
	{
	    content[iterator] = c;
	    iterator++;

	    if (iterator >= alloc_size)
	    {
		/* we need more space, should be rare,
		 * show a warning to prevent and correct if necessary */
		devel_debug ("Realloc is needed, if this message comes often, please contact the Grisbi team to improve the software ;-)");
		alloc_size = alloc_size + orig_size;
		content = g_realloc (content, alloc_size);

		if (!content)
		{
		    dialogue_error_memory ();
		    return FALSE;
		}
	    }
	}
    }
    while (!eof);

    content[iterator] = '\0';

    /* fill the returned values */
    *length = iterator;
    *file_content = content;

    gzclose (file);
    return TRUE;
}


/**
 * for a grisbi file before 0.5.1, switch the R and T transactions because T appears
 * called only for a file before 0.5.1
 *
 * \param
 *
 * \return
 * */
void switch_t_r ( void )
{
    /* cette fonction fait le tour des opérations et change le marquage T et R des opés */
    /*     R devient pointe=3 */
    /*     T devient pointe=2 */
    /*     à n'appeler que pour une version antérieure à 0.5.1 */

    GSList *list_tmp_transactions;

    if ( !gsb_data_account_get_accounts_amount () )
	return;

    devel_debug ( "switch_t_r");


    list_tmp_transactions = gsb_data_transaction_get_complete_transactions_list ();

    while ( list_tmp_transactions )
    {
	gint transaction_number_tmp;
	transaction_number_tmp = gsb_data_transaction_get_transaction_number (list_tmp_transactions -> data);

	switch ( gsb_data_transaction_get_marked_transaction (transaction_number_tmp))
	{
	    case 2 :
		gsb_data_transaction_set_marked_transaction ( transaction_number_tmp,
							      3 );
		break;
	    case 3:
		gsb_data_transaction_set_marked_transaction ( transaction_number_tmp,
							      2 );
		break;
	}
	list_tmp_transactions = list_tmp_transactions -> next;
    }
}



/**
 * create or delete a file ".name_of_file.swp" to check if the file is opened
 * already or not
 *
 * \param create_swp if we create or delete it
 *
 * \return TRUE if ok
 * */
gboolean gsb_file_util_modify_lock ( gboolean create_swp )
{
    gchar *lock_filename;
    gchar **tab_str;
    gint i;

    /* if the file was already opened and we don't force the saving, we do nothing */
    if ( (etat.fichier_deja_ouvert
	  &&
	  !etat.force_enregistrement)
	 ||
	 !nom_fichier_comptes
	 ||
	 !strlen(nom_fichier_comptes) )
	return TRUE;

    /* Check if nom_fichier_comptes exists.  If not, this is a new
     * file so don't try to lock it. */
    if (!g_file_test ( nom_fichier_comptes,
		       G_FILE_TEST_EXISTS ))
    {
	return FALSE;
    }

    /* Create the name of the lock file */
    tab_str = g_strsplit ( nom_fichier_comptes, G_DIR_SEPARATOR_S, 0 );

    i=0;

    while ( tab_str[i+1] )
	i++;

    tab_str[i] = g_strconcat ( 
#ifndef _WIN32
                              ".",
#endif
			       tab_str[i],
			       ".swp",
			       NULL );
    lock_filename = g_strjoinv ( G_DIR_SEPARATOR_S,
				 tab_str );
    g_strfreev ( tab_str );

    if ( create_swp )
    {
	/* now we create the lock file */

	FILE *fichier;

	/* check if the file lock exists */
	if (g_file_test ( lock_filename,
			  G_FILE_TEST_EXISTS ))
	{
	    if ( ! etat.force_enregistrement )
		dialog_message ( "account-already-opened", nom_fichier_comptes );

	    /* the lock is already created, return TRUE */
	    etat.fichier_deja_ouvert = 1;
	    return TRUE;
	}

	etat.fichier_deja_ouvert = 0;

	fichier = utf8_fopen ( lock_filename, "w" );

	if ( !fichier )
	{
	    gchar* tmpstr = g_strdup_printf (_("Cannot write lock file :'%s': %s"),
					     nom_fichier_comptes,
					     latin2utf8 (strerror(errno)));
	    dialogue_error ( tmpstr );
	    g_free ( tmpstr );
	    return FALSE;
	}

	fclose ( fichier );
	return TRUE;
    }
    else
    {
	/* delete the lock file */
	gint result;

	etat.fichier_deja_ouvert = 0;

	/* check if it exits, if not, just go away */
	if (!g_file_test ( lock_filename,
			   G_FILE_TEST_EXISTS ))
	    return TRUE;

	result = utf8_remove ( lock_filename );

	if ( result == -1 )
	{
	    gchar* tmpstr = g_strdup_printf (_("Cannot erase lock file :'%s': %s"),
					     nom_fichier_comptes,
					     latin2utf8 (strerror(errno)));
	    dialogue_error ( tmpstr );
	    g_free ( tmpstr );
	    return FALSE;
	}
	return TRUE;
    }
}

/**
 *
 * called when loading a file, if the permissions are not set only for the user
 * propose to change the permissions
 *
 * \param
 *
 * \return
 * */
void gsb_file_util_change_permissions ( void )
{
    /* On Windows, the chmod feature does not work: FAT does not
     * have right access permission notions , on NTFS it to
     * complicated to implement => the feature is removed from the
     * Windows version : for that the corresponding parameter
     * check box is not displayed and the paramater is forced to
     * not display msg. */
#ifndef _WIN32
    if ( question_conditional_yes_no ( "account-file-readable" ) == TRUE )
    {
	chmod ( nom_fichier_comptes, S_IRUSR | S_IWUSR );
    }

#endif /* _WIN32 */
}


