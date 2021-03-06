//
//  bls12_line_ate.c
//  elips_refactoring
//
//  Created by Y.Nanjo and M.A.A Khandaker on 2/6/18.
//  Copyright © 2018 ISec Lab. All rights reserved.
//

#include <ELiPS_bn_bls/bls12_line_ate.h>

void BLS12_ff_ltt(Fp12 *f,EFp2 *T,EFp *P,Fp *L){
    EFp2 Tmp_T;
    EFp2_init(&Tmp_T);
    Fp12 ff,ltt;
    Fp12_init(&ff);
    Fp12_init(&ltt);
    Fp2 A,B,C,D,E;
    Fp2_init(&A);
    Fp2_init(&B);
    Fp2_init(&C);
    Fp2_init(&D);
    Fp2_init(&E);
    EFp2_set(&Tmp_T,T);
    
    Fp12_sqr(&ff,f);
    
    //ltt
    Fp2_add(&A,&Tmp_T.y,&Tmp_T.y);        //A=1/(2*T.y)
    Fp2_inv(&A,&A);
    Fp2_sqr(&B,&Tmp_T.x);            //B=3(T.x)^2
    Fp2_mul_ui(&B,&B,3);
    Fp2_mul(&C,&A,&B);                //C=A*B
    Fp2_add(&D,&Tmp_T.x,&Tmp_T.x);        //D=2T.x
    Fp2_sqr(&T->x,&C);                //next_T.x=C^2-D
    Fp2_sub(&T->x,&T->x,&D);
    Fp2_mul(&E,&C,&Tmp_T.x);            //E=C*T.x-T.y
    Fp2_sub(&E,&E,&Tmp_T.y);
    Fp2_mul(&T->y,&C,&T->x);            //next_T.y=E-C*next_T.x
    Fp2_sub(&T->y,&E,&T->y);
    
    //set ltt
    Fp_set_ui(&ltt.x0.x0.x0,1);
    Fp2_set_neg(&ltt.x1.x2,&C);
    Fp2_inv_basis(&ltt.x1.x2,&ltt.x1.x2);
    Fp2_mul_mpz(&ltt.x1.x1,&E,L->x0);
    Fp2_inv_basis(&ltt.x1.x1,&ltt.x1.x1);
    
    BLS12_Pseudo_8_sparse_mul(f,&ff,&ltt);
    
    EFp2_clear(&Tmp_T);
    Fp2_clear(&A);
    Fp2_clear(&B);
    Fp2_clear(&C);
    Fp2_clear(&D);
    Fp2_clear(&E);
    Fp12_clear(&ff);
    Fp12_clear(&ltt);
}

void BLS12_f_ltq(Fp12 *f,EFp2 *T,EFp2 *Q,EFp *P,Fp *L){
    EFp2 Tmp_T;
    EFp2_init(&Tmp_T);
    Fp12 ltq;
    Fp12_init(&ltq);
    Fp2 A,B,C,D,E;
    Fp2_init(&A);
    Fp2_init(&B);
    Fp2_init(&C);
    Fp2_init(&D);
    Fp2_init(&E);
    EFp2_set(&Tmp_T,T);
    
    //ltq
    Fp2_sub(&A,&Q->x,&Tmp_T.x);        //A=(Q->x-T.x)^-1
    Fp2_inv(&A,&A);
    Fp2_sub(&B,&Q->y,&Tmp_T.y);        //B=(Q->y-T.y)
    Fp2_mul(&C,&A,&B);            //C=A*B
    Fp2_add(&D,&Tmp_T.x,&Q->x);        //D=Q->x+T.x
    Fp2_sqr(&T->x,&C);            //next_T.x=C^2-D
    Fp2_sub(&T->x,&T->x,&D);
    Fp2_mul(&E,&C,&Tmp_T.x);        //E=C*T.x-T.y
    Fp2_sub(&E,&E,&Tmp_T.y);
    Fp2_mul(&T->y,&C,&T->x);        //next_T.y=E-C*next_T.x
    Fp2_sub(&T->y,&E,&T->y);
    
    //set ltq
    Fp_set_ui(&ltq.x0.x0.x0,1);
    Fp2_set_neg(&ltq.x1.x2,&C);
    Fp2_inv_basis(&ltq.x1.x2,&ltq.x1.x2);
    Fp2_mul_mpz(&ltq.x1.x1,&E,L->x0);
    Fp2_inv_basis(&ltq.x1.x1,&ltq.x1.x1);
    
    BLS12_Pseudo_8_sparse_mul(f,f,&ltq);
    
    EFp2_clear(&Tmp_T);
    Fp12_clear(&ltq);
    Fp2_clear(&A);
    Fp2_clear(&B);
    Fp2_clear(&C);
    Fp2_clear(&D);
}
