#include<gsl/gsl_vector.h>
#include<gsl/gsl_vector_double.h>
#include<gsl/gsl_blas.h>
#include<gsl/gsl_matrix.h>
#include<math.h>
#include<gsl/gsl_blas.h>
#include<assert.h>
#include<gsl/gsl_linalg.h>
#include<stdio.h>
#define rnd (double)rand()/RAND_MAX

void show_matrix(gsl_matrix* A){
	int n = A->size1; //længde af vektorene
	int m = A->size2; // antal vektorer
	printf("The matrix looks like this\n");
	for(int i=0;i<n;i++){
		for(int j=0;j<m;j++){
			double x = gsl_matrix_get(A,i,j);
			printf("%g ",x);
		}
		printf("\n");
	}
}

double cdot(gsl_vector* A, gsl_vector* B){
	assert(A->size==B->size);	
	double result;
	for(int i = 0;i<A->size;i++){
		double b = gsl_vector_get(A,i)*gsl_vector_get(B,i);
		result += b;	
	}
	return result;
}


void show_vector(gsl_vector* V){
	int n = V->size;
	printf("The vector looks like\n");
	for(int i=0;i<n;i++){
		double x = gsl_vector_get(V,i);
		printf("%g\n",x);
	}
}
void GS_decomp(gsl_matrix* A,gsl_matrix* R){
	assert(A->size2 == R->size1 && A->size2 == R->size2);
	int N = A->size1;
	int M = A->size2;
	gsl_matrix* Q = gsl_matrix_alloc(N,M);	
	for(int i=0;i<M;i++){
		gsl_vector* q = gsl_vector_alloc(N);
		gsl_matrix_get_col(q,A,i);
		double x = gsl_blas_dnrm2(q);
		gsl_vector_scale(q,1/x);
		gsl_matrix_set_col(Q,i,q);
		for(int j=i+1;j<M;j++){
		gsl_vector* a = gsl_vector_alloc(N);
                gsl_matrix_get_col(a,A,j);
		double alpha = cdot(a,q);
		for(int i=0;i<a->size;i++){
		double aj = gsl_vector_get(a,i);
		double qi = gsl_vector_get(q,i);
		gsl_vector_set(a,j,aj-alpha*qi);
		}
		gsl_matrix_set_col(A,j,a);
		}
	}
	for(int i=0;i<M;i++){
		gsl_vector* a = gsl_vector_alloc(N);
		gsl_vector* q = gsl_vector_alloc(N);
                gsl_matrix_get_col(a,A,i);
		double Rii = gsl_blas_dnrm2(a);
		gsl_matrix_set(R,i,i,Rii);
		gsl_vector_memcpy(q,a);
		gsl_vector_scale(q,1/Rii);
		gsl_matrix_set_col(Q,i,q);
		for(int j=i+1;j<M;j++){
	  		gsl_matrix_get_col(a,A,j);
			double Rij = cdot(q,a);
			gsl_blas_daxpy(-Rij,q,a);
			gsl_matrix_set(R,i,j,Rij);
			gsl_matrix_set_col(A,j,a);
		}
	}
	gsl_matrix_memcpy(A,Q);
//	gsl_vector_free(q);
//	gsl_vector_free(a);
	gsl_matrix_free(Q);
}

int main(){
	int N =3;
	gsl_matrix* Q = gsl_matrix_alloc(N,N);	
	gsl_matrix* B = gsl_matrix_alloc(N,N);	
	gsl_matrix* R = gsl_matrix_alloc(N,N);	
	gsl_vector* V = gsl_vector_alloc(N);
        for(int i=0;i<N;i++){
    		for(int j=0;j<N;j++){
			gsl_matrix_set(Q,i,j,rnd);
		}
		gsl_vector_set(V,i,rnd);
	}
	show_matrix(Q);	
	gsl_matrix_memcpy(B,Q);
	printf("\n\n");
	show_vector(V);
	printf("\n\n");
	GS_decomp(Q,R);
	show_matrix(Q);	
	printf("\n\n");
	show_matrix(R);	
	printf("\n\n");
	gsl_matrix_free(Q);
	gsl_matrix_free(B);
	gsl_matrix_free(R);
	gsl_vector_free(V);
return 0;
}