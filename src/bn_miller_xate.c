//
//  bn_miller_xate.c
//  elips_refactoring
//
//  Created by Y.Nanjo and M.A.A Khandaker on 2/2/18.
//  Copyright © 2018 ISec Lab. All rights reserved.
//

#include <ELiPS_bn_bls/bn_miller_xate.h>


void Miller_algo_for_x_ate(Fp12 *ANS,EFp12 *P,EFp12 *Q){
    EFp2 T;
    EFp2_init(&T);
    EFp2 mapped_Q,mapped_Q_neg,mapped_Q1,mapped_Q2;
    EFp2_init(&mapped_Q);
    EFp2_init(&mapped_Q_neg);
    EFp2_init(&mapped_Q1);
    EFp2_init(&mapped_Q2);
    EFp mapped_P;
    EFp_init(&mapped_P);
    Fp12 f,tmp;
    Fp12_init(&f);
    Fp12_init(&tmp);
    Fp L;
    Fp_init(&L);
    int i;
    
    //set
    Fp_set(&mapped_P.x,&P->x.x0.x0.x0);    //set P
    Fp_set(&mapped_P.y,&P->y.x0.x0.x0);
    mapped_P.infinity=P->infinity;
    EFp12_to_EFp2(&mapped_Q,Q);//set mapped_Q
    mapped_Q.infinity=Q->infinity;
    
    Pseudo_8_sparse_mapping(&mapped_P,&mapped_Q,&L);
    
    EFp2_set(&mapped_Q_neg,&mapped_Q);//set mapped_Q_neg
    Fp2_set_neg(&mapped_Q_neg.y,&mapped_Q_neg.y);
    EFp2_set(&T,&mapped_Q);        //set T
    Fp12_set_ui(&f,0);            //set f
    Fp_set_ui(&f.x0.x0.x0,1);
    //miller
    for(i=BN_X_length-1; i>=0; i--){
        switch(X_binary[i]){
            case 0:
                ff_ltt(&f,&T,&mapped_P,&L);
                break;
            case 1:
                ff_ltt(&f,&T,&mapped_P,&L);
                f_ltq(&f,&T,&mapped_Q,&mapped_P,&L);
                break;
            case -1:
                ff_ltt(&f,&T,&mapped_P,&L);
                f_ltq(&f,&T,&mapped_Q_neg,&mapped_P,&L);
                break;
            default:
                break;
        }
    }
    
    Fp12_frobenius_map_p3(&tmp,&f);        //f←f*f^(p^3)
    Fp12_mul(&f,&tmp,&f);
    EFp2_skew_frobenius_map_p3(&mapped_Q1,&T);//mapped_Q1←T^(p^3)
    EFp2_set(&mapped_Q2,&T);
    f_ltq(&f,&mapped_Q2,&mapped_Q1,&mapped_P,&L);
    Fp12_frobenius_map_p10(&tmp,&f);        //f←f*f^(p^10)
    Fp12_mul(&f,&tmp,&f);
    EFp2_skew_frobenius_map_p10(&T,&mapped_Q2);//T←Q2^(p^10)
    f_ltq(&f,&T,&mapped_Q2,&mapped_P,&L);
    
    Fp12_set(ANS,&f);
    
    Fp12_clear(&tmp);
    Fp12_clear(&f);
    EFp2_clear(&T);
    EFp2_clear(&mapped_Q);
    EFp2_clear(&mapped_Q_neg);
    EFp2_clear(&mapped_Q1);
    EFp2_clear(&mapped_Q2);
    EFp_clear(&mapped_P);
    Fp_clear(&L);
}
