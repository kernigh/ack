/* T Y P E   E Q U I V A L E N C E */

static char *RcsId = "$Header$";

#include	<em_arith.h>
#include	<em_label.h>
#include	"type.h"
#include	"def.h"
#include	"Lpars.h"

int
TstTypeEquiv(tp1, tp2)
	register struct type *tp1, *tp2;
{
	/*	test if two types are equivalent. The only complication comes
		from the fact that for some procedures two declarations may
		be given: one in the specification module and one in the
		definition module.
	*/

	return     tp1 == tp2
		||
		   ( 
		     tp1 && tp1->tp_fund == PROCEDURE
		   &&
		     tp2 && tp2->tp_fund == PROCEDURE
		   &&
		     TstProcEquiv(tp1, tp2)
		   );

}

int
TstProcEquiv(tp1, tp2)
	register struct type *tp1, *tp2;
{
	/*	Test if two procedure types are equivalent. This routine
		may also be used for the testing of assignment compatibility
		between procedure variables and procedures.
	*/
	register struct paramlist *p1, *p2;

	if (!TstTypeEquiv(tp1->next, tp2->next)) return 0;
	p1 = tp1->prc_params;
	p2 = tp2->prc_params;
	while (p1 && p2) {
		if (p1->par_var != p2->par_var ||
		    !TstTypeEquiv(p1->par_type, p2->par_type)) return 0;
		p1 = p1->next;
		p2 = p2->next;
	}
	if (p1 != p2) return 0;
	return 1;
}

int
TstCompat(tp1, tp2)
	register struct type *tp1, *tp2;
{
	/*	test if two types are compatible. See section 6.3 of the
		Modula-2 Report for a definition of "compatible".
	*/
	if (TstTypeEquiv(tp1, tp2)) return 1;
	if (tp2->tp_fund == SUBRANGE) tp1 = tp1->next;
	if (tp2->tp_fund == SUBRANGE) tp1 = tp1->next;
	return	tp1 == tp2
	    ||
		(  tp1 == address_type
		&& 
	          (  tp2 == card_type
		  || tp2 == intorcard_type
		  || tp2->tp_fund == POINTER
		  )
		)
	    ||
		(  tp2 == address_type
		&& 
	          (  tp1 == card_type
		  || tp1 == intorcard_type
		  || tp1->tp_fund == POINTER
		  )
		)
	;
}
