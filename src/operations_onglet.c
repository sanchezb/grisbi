/*  Fichier qui s'occupe de former les différentes fenêtres de l'paned_onglet_operations des opés */
/*      paned_onglet_operations_operations.c */

/*     Copyright (C) 2000-2003  Cédric Auger */
/* 			cedric@grisbi.org */
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



/*START_INCLUDE*/
#include "operations_onglet.h"
#include "equilibrage.h"
#include "operations_liste.h"
#include "operations_formulaire.h"
#include "operations_comptes.h"
#include "gsb_account.h"
/*END_INCLUDE*/

/*START_STATIC*/
/*END_STATIC*/




GtkWidget *paned_onglet_operations;



/*START_EXTERN*/
extern GtkWidget *formulaire;
extern GtkWidget *frame_droite_bas;
extern GtkWidget *label_last_statement;
extern GtkWidget *notebook_comptes_equilibrage;
/*END_EXTERN*/



/** Create the page with the transactions list and accounts list
 * \param none
 * \return the page (GtkWidget)
 * */
GtkWidget *create_transaction_page ( void )
{
    GtkWidget *frame_gauche, *vbox, *frame_droite_haut, *fenetre_operations, *hbox, *label;

    paned_onglet_operations = gtk_hpaned_new ( );

    if ( !etat.largeur_colonne_comptes_operation )
	etat.largeur_colonne_comptes_operation = 200;

    gtk_paned_set_position ( GTK_PANED(paned_onglet_operations), etat.largeur_colonne_comptes_operation );
    gtk_container_set_border_width ( GTK_CONTAINER ( paned_onglet_operations ), 10 );
    gtk_widget_show ( paned_onglet_operations );

    /*   création de la fenetre des comptes / ventilation / équilibrage
	 à gauche */
    frame_gauche = gtk_frame_new ( NULL );
    gtk_frame_set_shadow_type ( GTK_FRAME ( frame_gauche ), GTK_SHADOW_IN );
    gtk_paned_pack1 ( GTK_PANED(paned_onglet_operations), frame_gauche, TRUE, TRUE );
    gtk_widget_show (frame_gauche);

    notebook_comptes_equilibrage = gtk_notebook_new ();
    gtk_notebook_set_show_tabs ( GTK_NOTEBOOK( notebook_comptes_equilibrage ),
				 FALSE );
    gtk_container_add ( GTK_CONTAINER ( frame_gauche ),
			notebook_comptes_equilibrage );
    gtk_widget_show ( notebook_comptes_equilibrage );

    /*  Création de la fenêtre des comptes */
    gtk_notebook_append_page ( GTK_NOTEBOOK ( notebook_comptes_equilibrage ),
			       creation_liste_comptes (),
			       gtk_label_new ( _("Accounts") ) );


    /* création de la fenetre de l'équilibrage */
    gtk_notebook_append_page ( GTK_NOTEBOOK ( notebook_comptes_equilibrage ),
			       creation_fenetre_equilibrage (),
			       gtk_label_new ( _("Reconciliation") ) );

    /* création de la partie droite */
    vbox = gtk_vbox_new ( FALSE, 10 );
    gtk_paned_pack2 ( GTK_PANED(paned_onglet_operations), vbox, TRUE, TRUE );
    gtk_widget_show ( vbox );

    /*  Création de la liste des opérations */
    frame_droite_haut = gtk_frame_new ( NULL );
    gtk_box_pack_start ( GTK_BOX ( vbox ), frame_droite_haut, TRUE, TRUE, 0 );
    gtk_frame_set_shadow_type ( GTK_FRAME ( frame_droite_haut ), GTK_SHADOW_IN );
    gtk_widget_show (frame_droite_haut);

    fenetre_operations = creation_fenetre_operations();
    gtk_container_set_border_width ( GTK_CONTAINER ( fenetre_operations ), 10);
    gtk_container_add ( GTK_CONTAINER ( frame_droite_haut ), fenetre_operations );
    gtk_widget_show ( fenetre_operations ); 

    /* We then create the edit form, inside a gtkexpander  */

    /* Expander has a composite label */
    hbox = gtk_hbox_new ( FALSE, 6 );
    label = gtk_label_new ( "" );
    gtk_label_set_markup_with_mnemonic ( GTK_LABEL ( label ), 
					 g_strconcat ( "<span weight=\"bold\">", 
						       _("Transaction _form"),
						       "</span>", NULL ) );
    gtk_box_pack_start ( GTK_BOX ( hbox ), label, FALSE, FALSE, 0 );
    label_last_statement = gtk_label_new ( "" );
    gtk_label_set_justify ( GTK_LABEL(label_last_statement), GTK_JUSTIFY_RIGHT ) ;
    gtk_misc_set_alignment (GTK_MISC (label_last_statement), 1, 1);
    gtk_box_pack_start ( GTK_BOX ( hbox ), label_last_statement, TRUE, TRUE, 0 );
    /* Kludge : otherwise, GtkExpander won't give us as many space
       as we need. */
    gtk_widget_set_size_request ( hbox, 2048, -1 );

    /* Create the expander */
    frame_droite_bas = gtk_expander_new ( "" );
    gtk_expander_set_expanded ( GTK_EXPANDER ( frame_droite_bas ), 
				etat.formulaire_toujours_affiche );
    gtk_expander_set_label_widget ( GTK_EXPANDER(frame_droite_bas), hbox );
    gtk_box_pack_start ( GTK_BOX ( fenetre_operations ), frame_droite_bas, 
			 FALSE, FALSE, 0 );
    gtk_widget_show_all (frame_droite_bas);

    /* Create form */
    formulaire = creation_formulaire ();
    gtk_container_set_border_width ( GTK_CONTAINER ( formulaire ), 10);
    gtk_container_add ( GTK_CONTAINER ( frame_droite_bas ), formulaire );
    gtk_widget_show (formulaire);


    /* for the current account : */
    /* we show the current form */
    remplissage_formulaire ( gsb_account_get_current_account ());

    /* we fill the marked amount and the total amount */

    mise_a_jour_labels_soldes ();

    return ( paned_onglet_operations );
}
/*****************************************************************************************************/



/* Local Variables: */
/* c-basic-offset: 4 */
/* End: */
