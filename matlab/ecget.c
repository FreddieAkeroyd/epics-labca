#include <mglue.h>

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
int      namelen,i;
char     name[100];
long    *buf=0;
int      nelems=0;
mxArray  *mxa;
double   *dptr;
    
    *plhs=0;

	/* check for one argument of type string */
	if ( nrhs != 1 ) {
		MEXERRPRINTF( "usage:  data = ecget( PVName );\n" );
		return;
	}
	if ( mxIsChar( prhs[0] ) != 1 ) {
		MEXERRPRINTF( "usage:  data = ecget( PVName );\n" );
		return;
	}

    namelen = (mxGetM(prhs[0]) * mxGetN(prhs[0]) * sizeof(mxChar)) + 1;
    
    if (namelen>=sizeof(name)) {
        MEXERRPRINTF("PV name too long\n");
        return;
    }
    
    mxGetString(prhs[0], name, namelen);
    
    ecdrget(name,&namelen,&buf,&nelems);

    if (!buf) {
        return;
    }
    
    if (!(mxa = mxCreateDoubleMatrix(1,nelems,mxREAL))){
        mxFree(buf); buf=0;
        MEXERRPRINTF("ecdrget: no memory");
        goto cleanup;
    }
    
    for (i=0, dptr=mxGetPr(mxa); i<nelems; i++, dptr++)
        *dptr = (double)buf[i];
    
    plhs[0] = mxa; mxa=0;
    

cleanup:
    if (buf) mxFree(buf);
	if (mxa) {
		mxDestroyArray(mxa);
	}
}