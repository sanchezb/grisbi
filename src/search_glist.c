/* ************************************************************************** */
/*   									      */
/*  					                                      */
/*                                                                            */
/*                                  search_glist.c                            */
/*                                                                            */
/*     Copyright (C)	2000-2003 C�dric Auger (cedric@grisbi.org)	      */
/*			2003-2004 Benjamin Drieu (bdrieu@april.org)	      */
/*			2003-2004 Alain Portal (dionysos@grisbi.org)	      */
/* 			http://www.grisbi.org				      */
/*                                                                            */
/*  This program is free software; you can redistribute it and/or modify      */
/*  it under the terms of the GNU General Public License as published by      */
/*  the Free Software Foundation; either version 2 of the License, or         */
/*  (at your option) any later version.                                       */
/*                                                                            */
/*  This program is distributed in the hope that it will be useful,           */
/*  but WITHOUT ANY WARRANTY; without even the implied warranty of            */
/*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             */
/*  GNU General Public License for more details.                              */
/*                                                                            */
/*  You should have received a copy of the GNU General Public License         */
/*  along with this program; if not, write to the Free Software               */
/*  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA */
/*                                                                            */
/* ************************************************************************** */

#include "include.h"
#include "structures.h"
#include "variables-extern.c"
#include "search_glist.h"



/* #include "accueil.h" */
/* #include "affichage.h" */
/* #include "affichage_liste.h" */
/* #include "banque.h" */
/* #include "barre_outils.h" */
/* #include "calendar.h" */
/* #include "categories_onglet.h" */
/* #include "comptes_gestion.h" */
/* #include "comptes_onglet.h" */
/* #include "comptes_traitements.h" */
/* #include "constants.h" */
/* #include "devises.h" */
/* #include "dialog.h" */
/* #include "echeancier_formulaire.h" */
/* #include "echeancier_liste.h" */
/* #include "equilibrage.h" */
/* #include "erreur.h" */
/* #include "etat_io.h" */
/* #include "etats_calculs.h" */
/* #include "etats_config.h" */
/* #include "etats_gtktable.h" */
/* #include "etats.h" */
/* #include "etats_latex.h" */
/* #include "etats_onglet.h" */
/* #include "etats_support.h" */
/* #include "exercice.h" */
/* #include "fenetre_principale.h" */
/* #include "fichier_configuration.h" */
/* #include "fichiers_gestion.h" */
/* #include "fichiers_io.h" */
/* #include "gtkcombofix.h" */
/* #include "gtk_list_button.h" */
/* #include "help.h" */
/* #include "html.h" */
/* #include "import.h" */
/* #include "imputation_budgetaire.h" */
/* #include "main.h" */
/* #include "menu.h" */
/* #include "ofx.h" */
/* #include "operations_classement.h" */
/* #include "operations_comptes.h" */
/* #include "operations_formulaire.h" */
/* #include "operations_liste.h" */
/* #include "operations_onglet.h" */
/* #include "parametres.h" */
/* #include "patienter.h" */
/* #include "print_config.h" */
/* #include "qif.h" */
/* #include "search_glist.h" */
/* #include "tiers_onglet.h" */
/* #include "traitement_variables.h" */
/* #include "type_operations.h" */
/* #include "utils.h" */
/* #include "ventilation.h" */








/***********************************************************************************************************/
/* Fonction recherche_categorie_par_nom */
/***********************************************************************************************************/

gint recherche_categorie_par_nom ( struct struct_categ *categorie,
				   gchar *categ )
{
    return ( g_strcasecmp ( categorie -> nom_categ,
			    categ ) );
}
/***********************************************************************************************************/


/***********************************************************************************************************/
/* Fonction recherche_sous_categorie_par_nom */
/***********************************************************************************************************/

gint recherche_sous_categorie_par_nom ( struct struct_sous_categ *sous_categorie,
					gchar *sous_categ )
{
    return ( g_strcasecmp ( sous_categorie -> nom_sous_categ,
			    sous_categ ) );
}
/***********************************************************************************************************/



/***************************************************************************************************/
/* Fonction  recherche_categorie_par_no */
/* appel�e par un g_slist_find_custom */
/* donne en arg la struct du tiers et le no du tiers recherch� */
/***************************************************************************************************/

gint recherche_categorie_par_no ( struct struct_categ *categorie,
				  gint *no_categorie )
{
    return (  categorie ->  no_categ != GPOINTER_TO_INT ( no_categorie ) );
}
/***************************************************************************************************/


/***************************************************************************************************/
/* Fonction  recherche_sous_categorie_par_no */
/* appel�e par un g_slist_find_custom */
/* donne en arg la struct du tiers et le no du tiers recherch� */
/***************************************************************************************************/

gint recherche_sous_categorie_par_no ( struct struct_sous_categ *sous_categorie,
				       gint *no_sous_categorie )
{
    return (  sous_categorie ->  no_sous_categ != GPOINTER_TO_INT ( no_sous_categorie ) );
}
/***************************************************************************************************/





/* ************************************************************************************************************ */
/* Fonction recherche_banque_par_no */
/* appel�e par g_slist_find_custom */
/* ************************************************************************************************************ */

gint recherche_banque_par_no ( struct struct_banque *banque,
			       gint *no_banque )
{

    return ( !( banque -> no_banque == GPOINTER_TO_INT ( no_banque )) );

}
/* ************************************************************************************************************ */


/* *********************************************************************************************************** */
/* cette fonction est appel�e pour chercher dans les �ch�ances si certaines sont  */
/* associ�es au compte en train d'�tre supprim� */
/* *********************************************************************************************************** */

gint cherche_compte_dans_echeances ( struct operation_echeance *echeance,
				     gint no_compte )
{

    return ( echeance -> compte != no_compte );

}
/* *********************************************************************************************************** */





/* ************************************************************************************************************ */
gint recherche_devise_par_nom ( struct struct_devise *devise,
				gchar *nom )
{

    return ( g_strcasecmp ( g_strstrip ( devise -> nom_devise ),
			    nom ) );

}
/* ************************************************************************************************************ */


/* ************************************************************************************************************ */
gint recherche_devise_par_code_iso ( struct struct_devise *devise,
				     gchar *nom )
{
    if ( devise -> code_iso4217_devise )
	return ( g_strcasecmp ( g_strstrip ( devise -> code_iso4217_devise ),
				nom ) );
    else
	return (-1);
}
/* ************************************************************************************************************ */



/***********************************************************************************************************/
/* Fonction recherche_devise_par_no */
/* appelée par un g_slist_find_custom */
/***********************************************************************************************************/

gint recherche_devise_par_no ( struct struct_devise *devise,
			       gint *no_devise )
{

    return ( devise -> no_devise != GPOINTER_TO_INT ( no_devise ));

}
/***********************************************************************************************************/




/*****************************************************************************/
gint recherche_echeance_par_no ( struct operation_echeance *echeance,
				 gint no_echeance )
{
    return ( !(echeance -> no_operation == no_echeance ));
}
/*****************************************************************************/


/******************************************************************************/
gint recherche_no_rapprochement_par_nom ( struct struct_no_rapprochement *rapprochement,
					  gchar *no_rap )
{
    return ( strcmp ( rapprochement -> nom_rapprochement,
		      no_rap ));
}
/******************************************************************************/

/******************************************************************************/
gint recherche_no_rapprochement_par_no ( struct struct_no_rapprochement *rapprochement,
					 gint *no_rap )
{

    return ( !(rapprochement -> no_rapprochement == GPOINTER_TO_INT ( no_rap )));

}
/******************************************************************************/


/******************************************************************************/
gint recherche_nom_dans_liste ( gchar *nom_liste,
				gchar *nom_test )
{
    return ( g_strcasecmp ( nom_liste,
			    nom_test ));
}
/******************************************************************************/


/*****************************************************************************************************/
gint recherche_etat_par_no ( struct struct_etat *etat,
			     gint *no_etat )
{
    return ( etat -> no_etat != GPOINTER_TO_INT (no_etat) );
}
/*****************************************************************************************************/


/* ************************************************************************************************************ */
gint recherche_exercice_par_nom ( struct struct_exercice *exercice,
				  gchar *nom )
{

    return ( g_strcasecmp ( g_strstrip ( exercice -> nom_exercice ),
			    nom ) );

}
/* ************************************************************************************************************ */



/***********************************************************************************************************/
/* Fonction recherche_exercice_par_no */
/* appel�e par un g_slist_find_custom */
/***********************************************************************************************************/

gint recherche_exercice_par_no ( struct struct_exercice *exercice,
				 gint *no_exercice )
{

    return ( exercice -> no_exercice != GPOINTER_TO_INT ( no_exercice ));

}
/***********************************************************************************************************/



/******************************************************************************/
/* Fonction recherche_operation_par_cheque					      */
/* appel�e par un slist_find_custom					      */
/* recherche une op� par son num�ro de cheque dans la liste des op�rations	      */
/******************************************************************************/
gint recherche_operation_par_cheque ( struct structure_operation *operation,
				      gint *no_chq )
{
    if ( operation -> contenu_type )
	return ( ! ( atoi (operation -> contenu_type) == GPOINTER_TO_INT ( no_chq ) ));
}
/******************************************************************************/





/* *******************************************************************************/
gint recherche_operation_par_id ( struct structure_operation *operation,
				      gchar *id_recherchee )
{
    if ( operation -> id_operation )
	return ( strcmp ( id_recherchee,
			  operation -> id_operation ));
    else
	return -1;
}
/* *******************************************************************************/





/***********************************************************************************************************/
/* Fonction recherche_imputation_par_nom */
/***********************************************************************************************************/

gint recherche_imputation_par_nom ( struct struct_imputation *imputation,
				    gchar *nom_imputation )
{
    return ( g_strcasecmp ( imputation -> nom_imputation,
			    nom_imputation ) );
}
/***********************************************************************************************************/


/***********************************************************************************************************/
/* Fonction recherche_sous_imputation_par_nom */
/***********************************************************************************************************/

gint recherche_sous_imputation_par_nom ( struct struct_sous_imputation *sous_imputation,
					 gchar *nom_sous_imputation )
{
    return ( g_strcasecmp ( sous_imputation -> nom_sous_imputation,
			    nom_sous_imputation ) );
}
/***********************************************************************************************************/



/***************************************************************************************************/
/* Fonction  recherche_imputation_par_no */
/* appel�e par un g_slist_find_custom */
/* donne en arg la struct du tiers et le no du tiers recherch� */
/***************************************************************************************************/

gint recherche_imputation_par_no ( struct struct_imputation *imputation,
				   gint *no_imputation )
{
    return (  imputation ->  no_imputation != GPOINTER_TO_INT ( no_imputation ) );
}
/***************************************************************************************************/


/***************************************************************************************************/
/* Fonction  recherche_imputation_par_no */
/* appel�e par un g_slist_find_custom */
/* donne en arg la struct du tiers et le no du tiers recherch� */
/***************************************************************************************************/

gint recherche_sous_imputation_par_no ( struct struct_sous_imputation *sous_imputation,
					gint *no_sous_imputation )
{
    return (  sous_imputation ->  no_sous_imputation != GPOINTER_TO_INT ( no_sous_imputation ) );
}
/***************************************************************************************************/




/******************************************************************************/
/* Fonction recherche_operation_par_no					      */
/* appel�e par un slist_find_custom					      */
/* recherche une op� par son num�ro d'op� dans la liste des op�rations	      */
/******************************************************************************/
gint recherche_operation_par_no ( struct structure_operation *operation,
				  gint *no_ope )
{
    return ( ! ( operation -> no_operation == GPOINTER_TO_INT ( no_ope ) ));
}
/******************************************************************************/


/***************************************************************************************************/
/* Fonction recherche tiers par no */
/* appel�e par un g_slist_find_custom */
/* donne en arg la struct du tiers et le no du tiers recherch� */
/***************************************************************************************************/

gint recherche_tiers_par_no ( struct struct_tiers *tiers,
			      gint *no_tiers )
{
    return ( tiers -> no_tiers != GPOINTER_TO_INT ( no_tiers ) );
}
/***************************************************************************************************/



/* **************************************************************************************************** */
/* Fonction recherche_tiers_par_nom */
/* appel�e par un g_slist_find_custom */
/* prend en arg la struct du tiers et le nom du tiers recherch� */
/* **************************************************************************************************** */

gint recherche_tiers_par_nom ( struct struct_tiers *tiers,
			       gchar *ancien_tiers )
{

    return ( g_strcasecmp ( tiers -> nom_tiers,
			    ancien_tiers ) );
}
/* **************************************************************************************************** */


/* ************************************************************************************************************** */
gint recherche_type_ope_par_no ( struct struct_type_ope *type_ope,
				 gint *no_type )
{

    return ( !(type_ope->no_type == GPOINTER_TO_INT(no_type)) );

}
/* ************************************************************************************************************** */



