void dialogue ( gchar * texte_dialogue );
void dialogue_hint ( gchar *text, gchar *hint );
void dialogue_error ( gchar *text );
void dialogue_error_hint ( gchar *text, gchar *hint );
void dialogue_warning ( gchar *text );
void dialogue_warning_hint ( gchar *text, gchar * hint );
void dialogue_special ( GtkMessageType param, gchar * text );
GtkWidget * dialogue_special_no_run ( GtkMessageType param, GtkButtonsType,
				      gchar * text );
void dialogue_conditional ( gchar *text, int * var );
void dialogue_conditional_hint ( gchar *hint, gchar * text, int * var );
gboolean blocage_boites_dialogues ( GtkWidget *dialog, gpointer null );
gboolean question ( gchar *texte );
gboolean question_yes_no ( gchar *texte );
gboolean question_yes_no_hint ( gchar *hint, gchar *text );
gchar *demande_texte ( gchar *titre_fenetre, gchar *question );
gchar * make_hint ( gchar * hint, gchar * text );
