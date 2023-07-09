/*
 * Information about fonts in use
 */

#include "wily.h"

static FontUse *fonts = 0;
FontUse *varfont, *fixedfont;

static FontUse *
newfont(char *name)
{
	Font *fnt;
	FontUse *use;

	fnt = rdfontfile(name, screen.ldepth);
	if (!fnt || charwidth(fnt, (Rune) ' ') == 0) {
		Subfont *subfont = getsubfont(name);
		fnt = 0;
		if (subfont)
			fnt = mkfont(subfont);
	}
	if (!fnt)
		return 0;
	if ((use = malloc(sizeof(FontUse))) == 0) {
		ffree(fnt);
		return 0;
	}
	if ((use->name = strdup(name)) == 0) {
		free(use);
		ffree(fnt);
		return 0;
	}
	use->font = fnt;
	use->ref = 1;
	use->next = fonts;
	fonts = use;
	return use;
}

void
initfont(char *fixed)
{
	if ((varfont = malloc(sizeof(FontUse))) == 0)
		fatal("couldn't allocate memory");
	varfont->name = font->name;
	varfont->font = font;
	varfont->ref = 1;
	varfont->next = 0;
	fonts = varfont;
	if ((fixedfont = newfont(fixed)) == 0)
		fatal("couldn't allocate fixed-width font %s", fixed);
}

void
setdefaultfont(FontUse **def, char *name)
{
	FontUse *newfont;

	if (strcmp((*def)->name, name) == 0)
		return;
	if ((newfont = findfont(name)) == 0)
		return;
	releasefont(*def);
	*def = newfont;
}

FontUse *
findfont(char *name)
{
	FontUse *use;

	for (use = fonts; use; use = use->next)
		if (strcmp(name, use->name) == 0) {
			usefont(use);
			return use;
		}
	return newfont(name);
}

void
usefont(FontUse *use)
{
	++use->ref;
}

void
releasefont(FontUse *use)
{
	FontUse **p, *unused;

	if (--use->ref != 0)
		return;
	for (p = &fonts; *p && *p != use; p = &(*p)->next)
		;
	if (*p) {
		unused = *p;
		*p = unused->next;
		ffree(unused->font);
		free(unused->name);
		free(unused);
	}
}
