#ifndef _TIERS_ONGLET_H
#define _TIERS_ONGLET_H (1)
/* START_INCLUDE_H */
/* END_INCLUDE_H */


/* START_DECLARATION */
gint classement_alphabetique_tree ( GtkWidget *tree,
				    GtkCListRow *ligne_1,
				    GtkCListRow *ligne_2 );
gboolean gsb_payee_update_combofix ( void );
GtkWidget *onglet_tiers ( void );
void payee_fill_tree ( void );
/* END_DECLARATION */
#endif
