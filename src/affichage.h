GtkWidget * onglet_display_transaction_list ( void );
GtkWidget * onglet_display_fonts ( void );
gboolean change_choix_utilise_logo ( GtkWidget *check_button,
				     GtkWidget *hbox );
gboolean change_choix_utilise_fonte_liste ( GtkWidget *check_button,
					    GtkWidget *vbox );
GtkWidget *onglet_display_addresses ( void );
GtkWidget * onglet_display_transaction_form ( void );
void deplacement_haut ( void );
void deplacement_bas ( void );
void choix_fonte ( GtkWidget *bouton,
		   gchar *fonte,
		   gpointer null );
void modification_logo_accueil ( void );
void update_font_button(GtkWidget * name_label,
			GtkWidget * size_label,
			gchar * fontname);
gboolean init_fonts ( GtkWidget * button,
		      gpointer user_data);
gboolean update_homepage_title ( GtkEntry *, gchar *, gint, gint * );

