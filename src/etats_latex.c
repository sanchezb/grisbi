/*  Fichier qui s'occupe d'afficher les �tats via une impression latex */
/*      etats_latex.c */

/*     Copyright (C)	2004 Benjamin Drieu (bdrieu@april.org)		      */
/* 			http://www.grisbi.org				      */

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
#include "structures.h"
#include "etats_latex.h"

#include "dialog.h"
#include "etats.h"



int lastline = 0;
int lastcol = 0;
int last_is_hsep = 0;
FILE * out;
struct print_config * printer_config;
gchar * tempname;


struct struct_etat_affichage latex_affichage = {
    latex_initialise,
    latex_finish,
    latex_attach_hsep,
    latex_attach_vsep,
    latex_attach_label,
};

extern struct struct_etat *etat_courant;
extern gint nb_colonnes;

/**
 * Backend function that is responsible for printing a label at a
 * specific place of the report, using text attibutes.
 *
 * \param text Text to print
 * \param properties bit field for text properties.  Can be:
 *        - TEXT_BOLD: text will be printed in bold
 *        - TEXT_ITALIC: text will be printed in intalic
 *        - TEXT_HUGE: text will be have a huge face
 *        - TEXT_LARGE: text will be have a large face
 *        - TEXT_SMALL: text will be have a small face
 * \param x left position of the label
 * \param y top position of the label
 * \param x2 right position of the label
 * \param y2 bottom position of the label
 * \param alignment how the text will be aligned in the cell
 * \param ope a pointer to a transaction to link to (not used as latex
 *            backend is not interactive)
 */
void latex_attach_label ( gchar * text, gdouble properties, int x, int x2, int y, int y2, 
			  enum alignement align, struct structure_operation * ope )
{
    int pad, realsize, realcolumns;

    if ( !text )
	text = "";

    if ( y >= lastline )
    {
	lastcol = 0;
	lastline = y2;
	if ( ! last_is_hsep )
	{
	    fprintf ( out, "\\\\\n" );
	}
    }

    for ( pad = lastcol ; pad < x ; pad ++ )
	fprintf ( out, "&" );

    if ( (x2 - x) > 1 )
	fprintf ( out, "\\multicolumn{%d}{l}{", (x2 - x) );

    realsize = (x2 - x);
    if ( realsize > 1 )
    {
	if ( etat_courant -> afficher_opes )
	{
	    realsize /= 2;
	    if ( x == 0 )
		realsize ++;
	}
    }
    if ( etat_courant -> afficher_opes )
	realcolumns = (float)((nb_colonnes / 2) + 1);
    else 
	realcolumns = nb_colonnes;

    fprintf ( out, 
	      "\\begin{boxedminipage}{%f\\text%s}\n", 
	      (float) realsize / (float) realcolumns,
	      ( etat.print_config.orientation == LANDSCAPE ? "height" : "width") );

    switch ( align )
    {
	case LEFT:
	    fprintf ( out, "\\raggedright\n" );
	    break;

	case RIGHT:
	    fprintf ( out, "\\raggedleft\n" );
	    break;

	case CENTER:
	    break;
    }

    if ( ((int) properties) & TEXT_BOLD )
    {
	fprintf ( out, "\\bf\n");
    }
    if ( ((int) properties) & TEXT_ITALIC )
    {
	fprintf ( out, "\\em\n");
    }
    if ( ((int) properties) & TEXT_HUGE )
    {
	fprintf ( out, "\\huge\n");
    }
    if ( ((int) properties) & TEXT_LARGE )
    {
	fprintf ( out, "\\Large\n");
    }
    if ( ((int) properties) & TEXT_SMALL )
    {
	fprintf ( out, "\\small\n");
    }

    latex_safe(text);
    fprintf ( out, "\\end{boxedminipage}" );

    if ( (x2 - x) > 1 )
	fprintf ( out, "}\n" );

    fprintf ( out, "&" );

    last_is_hsep = 0;
    lastcol = x2;
}



/**
 * Backend function that is responsible for printing a vertical
 * separator (i.e. a vertical line).
 *
 * \param x left position of the separator
 * \param y top position of the separator
 * \param x2 right position of the separator
 * \param y2 bottom position of the separator
 */
void latex_attach_vsep ( int x, int x2, int y, int y2)
{
    int pad;

    if ( y >= lastline )
    {
	if ( ! last_is_hsep )
	    fprintf ( out, "\\\\\n" );
	lastline = y2;
    }

    for ( pad = lastcol ; pad < x ; pad ++ )
	fprintf ( out, "&" );

    fprintf ( out, "{\\vrule}&" );

    last_is_hsep = 0;
    lastcol = x2;
}



/**
 * Backend function that is responsible for printing an horizontal
 * separator (i.e. an horizontal line).
 *
 * \param x left position of the separator
 * \param y top position of the separator
 * \param x2 right position of the separator
 * \param y2 bottom position of the separator
 */
void latex_attach_hsep ( int x, int x2, int y, int y2)
{
    if ( ! last_is_hsep )
	fprintf ( out, "\\\\\n" );
    lastline = y2;

    fprintf ( out, "\\hline\n" );

    last_is_hsep = 1;
    lastcol = x2;
}



/**
 * Initialization function for the latex backend.
 *
 * \return TRUE on succes, FALSE otherwise.
 */
gint latex_initialise (GSList * opes_selectionnees)
{
    gfloat colwidth, real_width;
    gchar * filename;
    int i;

    if ( ! print_config() )
	return FALSE;

    if ( etat.print_config.printer || etat.print_config.filetype == POSTSCRIPT_FILE )
    {
	tempname = g_strdup_printf ( "gsbpt%05d", g_random_int_range (0,99999) );
	filename =  g_strdup_printf ( "%s.tex", tempname );
    }
    else
    {
	filename = etat.print_config.printer_filename;
    }

    out = fopen ( filename, "w+x" );
    if ( ! out )
    {
	dialogue_error ( g_strdup_printf ("File '%s' already exists", filename ));;
	return FALSE;
    }

    fprintf (out, 
	     "\\documentclass{article}\n\n"
	     "\\special{! TeXDict begin /landplus90{true}store end }\n"
	     "\\usepackage{a4}\n"
	     "\\usepackage[utf8]{inputenc}\n"
	     "\\usepackage{eurosym}\n"
	     "\\usepackage{boxedminipage}\n"
	     "\\usepackage{longtable}\n"
	     "\\usepackage{vmargin}\n"
	     "\\usepackage[T1]{fontenc}\n");

    if ( etat.print_config.orientation == LANDSCAPE )
    {
	fprintf (out, "\\usepackage{portland}\n");
    }

    fprintf (out,
	     "\\setpapersize{%s}\n"
	     "\\setmarginsrb{1cm}{1cm}{1cm}{1cm}{0cm}{0cm}{0cm}{0cm}\n\n"
	     "\\begin{document}\n\n", etat.print_config.paper_config.name);

    if ( etat.print_config.orientation == LANDSCAPE )
      {
	fprintf (out, "\\landscape\n\n");
      }
	  
    fprintf (out,
	     "\\fboxsep \\parskip\n"
	     "\\fboxrule 0pt\n"
	     "\\tabcolsep 0pt\n"
	     "\\begin{longtable}[l]{" );

    if ( etat.print_config.orientation == LANDSCAPE )
    {
	real_width = ((etat.print_config.paper_config.height-20)/10);
    }
    else
    {
	real_width = ((etat.print_config.paper_config.width-20)/10);
    }

    if ( etat_courant -> afficher_opes )
    {
	colwidth = real_width / ((float) (nb_colonnes / 2) + 1 );
	fprintf ( out, "p{%fcm}", colwidth);
	for (i = 0 ; i < nb_colonnes/2 ; i++)
	{
	    fprintf ( out, "p{%fcm}p{1pt}", colwidth );
	}
	fprintf ( out, "}" );
    }
    else 
    {
	colwidth = real_width / (float) nb_colonnes;
	for (i = 0 ; i < nb_colonnes ; i++)
	{
	    fprintf (out, "p{%fcm}", colwidth);
	}
	fprintf (out, "p{1pt}}\n");
    }

    return TRUE;
}



/**
 * Destructor function for the latex backend.
 *
 * \return TRUE on success, FALSE otherwise.
 */
gint latex_finish ()
{
    gchar * command;

    fprintf (out, "\n"
	     "\\end{longtable}\n"
	     "\\end{document}\n");
    fclose (out);

    if ( etat.print_config.printer || etat.print_config.filetype == POSTSCRIPT_FILE )
    {
	command = g_strdup_printf ( "%s -interaction=nonstopmode %s.tex", etat.latex_command, tempname );
	if ( system ( command ) > 0 )
	    dialogue_error ( _("LaTeX run was unable to complete, see console output for details.") );
	else 
	{
	  command = g_strdup_printf ( "%s %s %s.dvi -o %s",  etat.dvips_command,
				      ( etat.print_config.orientation == LANDSCAPE ? "-t landscape" : ""),
				      tempname,
				      (etat.print_config.printer ? 
				       (g_strconcat ( tempname, ".ps", NULL )) : 
				       etat.print_config.printer_filename) );
	    unlink ( g_strdup_printf ("%s.tex", tempname) );
	    unlink ( g_strdup_printf ("%s.aux", tempname) );
	    unlink ( g_strdup_printf ("%s.log", tempname) );
	    if ( !system ( command ) )
	    {
		if ( etat.print_config.printer )
		{
		    command = g_strdup_printf ( "%s %s.ps", etat.print_config.printer_name, 
						tempname );
		    if ( system ( command ) )
		    {
			dialogue_error ( _("Cannot send job to printer") );
		    }
		}
	    }
	    else
	    {
		dialogue_error ( _("dvips was unable to complete, see console output for details.") );
	    }
	    unlink ( g_strdup_printf ("%s.dvi", tempname) );
	}

	if ( etat.print_config.printer )
	{
	    unlink ( g_strdup_printf ("%s.ps", tempname) );
	}

	g_free ( tempname );
    }  

    return 1;
}



/** 
 * Print a latex safe string into the out file descriptor.  All chars
 * that cannot be printed via latex are converted to their latex
 * equivalent (i.e. backslashes are escaped).
 *
 * \param text Text to print.
 */
void latex_safe ( gchar * text ) 
{
    gboolean start = 1;

    if ( ! text || ! strlen(text))
	return;

    for ( ; * text; text ++ )
    {
	switch ( * text )
	{
	    /* FIXME: this is very iso8859-1 centric */
	    case '�':
		if ( *(text+1) == '�' )
		{
		    fprintf ( out, "<<" );
		    text++;
		}
		else if ( *(text+1) == '�' )
		{
		    fprintf ( out, ">>" );
		    text++;
		}
		else 
		{
		    fprintf ( out, "%c", *text );
		}
		break;

	    case ' ':
		if ( start )
		    fprintf ( out, "~" );
		else
		    fprintf ( out, "%c", *text );
		break;

	    case '&':
	    case '\\':
		fprintf ( out, "\\" );

	    default:
		start = 0;
		fprintf ( out, "%c", *text );
		break;
	}
    }

}
