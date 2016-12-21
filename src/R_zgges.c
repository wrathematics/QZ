/* This is a C wrapper for "zgges.f" and called by "qz_zgges.r". */

#include "R_qz_global.h"

static int fake_selctg(Rcomplex *alpha, Rcomplex *beta){
	return 0;
} /* End of fake_sectg(). */

SEXP R_zgges(SEXP JOBVSL, SEXP JOBVSR, SEXP SORT, SEXP SELCTG, SEXP N,
		SEXP A, SEXP LDA, SEXP B, SEXP LDB, SEXP SDIM,
		SEXP ALPHA, SEXP BETA, SEXP VSL, SEXP LDVSL, SEXP VSR,
		SEXP LDVSR, SEXP WORK, SEXP LWORK, SEXP RWORK, SEXP BWORK,
		SEXP INFO){
	int n = INTEGER(N)[0], total_length;
	SEXP RET, RET_NAMES, S, T;
	char CS_JOBVSL = CHARPT(JOBVSL, 0)[0], CS_JOBVSR = CHARPT(JOBVSR, 0)[0],
		CS_SORT = CHARPT(SORT, 0)[0];

	/* Protect R objects. */
	PROTECT(RET = allocVector(VECSXP, 2));
	PROTECT(RET_NAMES = allocVector(STRSXP, 2));
	PROTECT(S = allocMatrix(CPLXSXP, n, n));
	PROTECT(T = allocMatrix(CPLXSXP, n, n));

	SET_VECTOR_ELT(RET, 0, S);
	SET_VECTOR_ELT(RET, 1, T);
	SET_STRING_ELT(RET_NAMES, 0, mkChar("S")); 
	SET_STRING_ELT(RET_NAMES, 1, mkChar("T")); 
	setAttrib(RET, R_NamesSymbol, RET_NAMES);

	/* COpy A and B since zgges writes in place. */
	total_length = n * n;
	Memcpy(COMPLEX(S), COMPLEX(A), total_length);
	Memcpy(COMPLEX(T), COMPLEX(B), total_length);

	/* Call Fortran. */
	if(CS_JOBVSL == 'V' && CS_JOBVSR == 'V' && CS_SORT == 'N'){
		F77_CALL(zgges)("V", "V", "N",
			fake_selctg, INTEGER(N), COMPLEX(S), INTEGER(LDA),
			COMPLEX(T), INTEGER(LDB), INTEGER(SDIM),
			COMPLEX(ALPHA), COMPLEX(BETA), COMPLEX(VSL),
			INTEGER(LDVSL),
			COMPLEX(VSR), INTEGER(LDVSR), COMPLEX(WORK),
			INTEGER(LWORK),
			REAL(RWORK), INTEGER(BWORK), INTEGER(INFO));
	} else if(CS_JOBVSL == 'N' && CS_JOBVSR == 'V' && CS_SORT == 'N'){
		F77_CALL(zgges)("N", "V", "N",
			fake_selctg, INTEGER(N), COMPLEX(S), INTEGER(LDA),
			COMPLEX(T), INTEGER(LDB), INTEGER(SDIM),
			COMPLEX(ALPHA), COMPLEX(BETA), COMPLEX(VSL),
			INTEGER(LDVSL),
			COMPLEX(VSR), INTEGER(LDVSR), COMPLEX(WORK),
			INTEGER(LWORK),
			REAL(RWORK), INTEGER(BWORK), INTEGER(INFO));
	} else if(CS_JOBVSL == 'V' && CS_JOBVSR == 'N' && CS_SORT == 'N'){
		F77_CALL(zgges)("V", "N", "N",
			fake_selctg, INTEGER(N), COMPLEX(S), INTEGER(LDA),
			COMPLEX(T), INTEGER(LDB), INTEGER(SDIM),
			COMPLEX(ALPHA), COMPLEX(BETA), COMPLEX(VSL),
			INTEGER(LDVSL),
			COMPLEX(VSR), INTEGER(LDVSR), COMPLEX(WORK),
			INTEGER(LWORK),
			REAL(RWORK), INTEGER(BWORK), INTEGER(INFO));
	} else if(CS_JOBVSL == 'N' && CS_JOBVSR == 'N' && CS_SORT == 'N'){
		F77_CALL(zgges)("N", "N", "N",
			fake_selctg, INTEGER(N), COMPLEX(S), INTEGER(LDA),
			COMPLEX(T), INTEGER(LDB), INTEGER(SDIM),
			COMPLEX(ALPHA), COMPLEX(BETA), COMPLEX(VSL),
			INTEGER(LDVSL),
			COMPLEX(VSR), INTEGER(LDVSR), COMPLEX(WORK),
			INTEGER(LWORK),
			REAL(RWORK), INTEGER(BWORK), INTEGER(INFO));
	} else{
		REprintf("Input (CHARACTER) types are not implemented.\n");
	}

	/* Return. */
	UNPROTECT(4);
	return(RET);
} /* End of R_zgges(). */

