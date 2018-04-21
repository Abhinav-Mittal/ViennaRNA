#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <string.h>
#include <limits.h>

#include "ViennaRNA/fold_vars.h"
#include "ViennaRNA/energy_par.h"
#include "ViennaRNA/utils.h"
#include "ViennaRNA/alphabet.h"
#include "ViennaRNA/constraints.h"
#include "ViennaRNA/gquad.h"
#include "ViennaRNA/structured_domains.h"
#include "ViennaRNA/unstructured_domains.h"
#include "ViennaRNA/exterior_loops.h"

#ifdef __GNUC__
# define INLINE inline
#else
# define INLINE
#endif

#include "exterior_loops_hc.inc"
#include "exterior_loops_sc.inc"

/*
 #################################
 # PRIVATE FUNCTION DECLARATIONS #
 #################################
 */
PRIVATE INLINE int
reduce_f5_up(vrna_fold_compound_t       *vc,
             int                        j,
             vrna_callback_hc_evaluate  *evaluate,
             struct default_data        *hc_dat_local,
             struct sc_wrapper_f5       *sc_wrapper);


PRIVATE INLINE int
reduce_f3_up(vrna_fold_compound_t       *vc,
             int                        i,
             vrna_callback_hc_evaluate  *evaluate,
             struct default_data        *hc_dat_local,
             struct sc_wrapper_f3       *sc_wrapper);


PRIVATE INLINE int *
get_stem_contributions_d0(vrna_fold_compound_t      *vc,
                          int                       j,
                          vrna_callback_hc_evaluate *evaluate,
                          struct default_data       *hc_dat_local,
                          struct sc_wrapper_f5      *sc_wrapper);


PRIVATE INLINE int *
f3_get_stem_contributions_d0(vrna_fold_compound_t       *vc,
                             int                        i,
                             vrna_callback_hc_evaluate  *evaluate,
                             struct default_data        *hc_dat_local,
                             struct sc_wrapper_f3       *sc_wrapper);


PRIVATE INLINE int *
get_stem_contributions_d2(vrna_fold_compound_t      *vc,
                          int                       j,
                          vrna_callback_hc_evaluate *evaluate,
                          struct default_data       *hc_dat_local,
                          struct sc_wrapper_f5      *sc_wrapper);


PRIVATE INLINE int *
f3_get_stem_contributions_d2(vrna_fold_compound_t       *vc,
                             int                        i,
                             vrna_callback_hc_evaluate  *evaluate,
                             struct default_data        *hc_dat_local,
                             struct sc_wrapper_f3       *sc_wrapper);


PRIVATE INLINE int *
f5_get_stem_contributions_d5(vrna_fold_compound_t       *vc,
                             int                        j,
                             vrna_callback_hc_evaluate  *evaluate,
                             struct default_data        *hc_dat_local,
                             struct sc_wrapper_f5       *sc_wrapper);


PRIVATE INLINE int *
f3_get_stem_contributions_d3(vrna_fold_compound_t       *vc,
                             int                        i,
                             vrna_callback_hc_evaluate  *evaluate,
                             struct default_data        *hc_dat_local,
                             struct sc_wrapper_f3       *sc_wrapper);


PRIVATE INLINE int *
f5_get_stem_contributions_d3(vrna_fold_compound_t       *vc,
                             int                        j,
                             vrna_callback_hc_evaluate  *evaluate,
                             struct default_data        *hc_dat_local,
                             struct sc_wrapper_f5       *sc_wrapper);


PRIVATE INLINE int *
f3_get_stem_contributions_d5(vrna_fold_compound_t       *vc,
                             int                        i,
                             vrna_callback_hc_evaluate  *evaluate,
                             struct default_data        *hc_dat_local,
                             struct sc_wrapper_f3       *sc_wrapper);


PRIVATE INLINE int *
f5_get_stem_contributions_d53(vrna_fold_compound_t      *vc,
                              int                       j,
                              vrna_callback_hc_evaluate *evaluate,
                              struct default_data       *hc_dat_local,
                              struct sc_wrapper_f5      *sc_wrapper);


PRIVATE INLINE int *
f3_get_stem_contributions_d53(vrna_fold_compound_t      *vc,
                              int                       i,
                              vrna_callback_hc_evaluate *evaluate,
                              struct default_data       *hc_dat_local,
                              struct sc_wrapper_f3      *sc_wrapper);


PRIVATE INLINE int
decompose_f5_ext_stem(vrna_fold_compound_t  *vc,
                      int                   j,
                      int                   *stems);


PRIVATE INLINE int
decompose_f3_ext_stem(vrna_fold_compound_t  *vc,
                      int                   i,
                      int                   max_j,
                      int                   *stems);


PRIVATE INLINE int
decompose_f5_ext_stem_d0(vrna_fold_compound_t       *vc,
                         int                        j,
                         vrna_callback_hc_evaluate  *evaluate,
                         struct default_data        *hc_dat_local,
                         struct sc_wrapper_f5       *sc_wrapper);


PRIVATE INLINE int
decompose_f3_ext_stem_d0(vrna_fold_compound_t       *vc,
                         int                        i,
                         vrna_callback_hc_evaluate  *evaluate,
                         struct default_data        *hc_dat_local,
                         struct sc_wrapper_f3       *sc_wrapper);


PRIVATE INLINE int
decompose_f5_ext_stem_d2(vrna_fold_compound_t       *vc,
                         int                        j,
                         vrna_callback_hc_evaluate  *evaluate,
                         struct default_data        *hc_dat_local,
                         struct sc_wrapper_f5       *sc_wrapper);


PRIVATE INLINE int
decompose_f3_ext_stem_d2(vrna_fold_compound_t       *vc,
                         int                        i,
                         vrna_callback_hc_evaluate  *evaluate,
                         struct default_data        *hc_dat_local,
                         struct sc_wrapper_f3       *sc_wrapper);


PRIVATE INLINE int
decompose_f5_ext_stem_d1(vrna_fold_compound_t       *vc,
                         int                        j,
                         vrna_callback_hc_evaluate  *evaluate,
                         struct default_data        *hc_dat_local,
                         struct sc_wrapper_f5       *sc_wrapper);


PRIVATE INLINE int
decompose_f3_ext_stem_d1(vrna_fold_compound_t       *vc,
                         int                        i,
                         vrna_callback_hc_evaluate  *evaluate,
                         struct default_data        *hc_dat_local,
                         struct sc_wrapper_f3       *sc_wrapper);


PRIVATE INLINE int
add_f5_gquad(vrna_fold_compound_t       *vc,
             int                        j,
             vrna_callback_hc_evaluate  *evaluate,
             struct default_data        *hc_dat_local,
             struct sc_wrapper_f5       *sc_wrapper);


PRIVATE INLINE int
add_f3_gquad(vrna_fold_compound_t       *vc,
             int                        i,
             vrna_callback_hc_evaluate  *evaluate,
             struct default_data        *hc_dat_local,
             struct sc_wrapper_f3       *sc_wrapper);


/*
 #################################
 # BEGIN OF FUNCTION DEFINITIONS #
 #################################
 */
PUBLIC int
vrna_E_ext_loop_5(vrna_fold_compound_t *fc)
{
  if (fc) {
    int                       en, j, length, *f5, dangle_model, with_gquad, turn;
    vrna_param_t              *P;
    vrna_callback_hc_evaluate *evaluate;
    struct default_data       hc_dat_local;
    struct sc_wrapper_f5      sc_wrapper;
    vrna_gr_aux_t             *grammar;

    length        = (int)fc->length;
    f5            = fc->matrices->f5;
    P             = fc->params;
    dangle_model  = P->model_details.dangles;
    with_gquad    = P->model_details.gquad;
    turn          = P->model_details.min_loop_size;
    evaluate      = prepare_hc_default(fc, &hc_dat_local);

    init_sc_wrapper(fc, &sc_wrapper);

    f5[0] = 0;
    for (j = 1; j <= turn + 1; j++)
      f5[j] = reduce_f5_up(fc, j, evaluate, &hc_dat_local, &sc_wrapper);

    if ((grammar) && (grammar->cb_aux_f))
      for (j = 1; j <= turn + 1; j++) {
        en = grammar->cb_aux_f(fc, 1, j, grammar->auxdata);
        f5[j] = MIN2(f5[j], en);
      }

    /*
     *  duplicated code may be faster than conditions inside loop or even
     *  using a function pointer ;)
     */
    switch (dangle_model) {
      case 2:
        for (j = turn + 2; j <= length; j++) {
          /* extend previous solution(s) by adding an unpaired region */
          f5[j] = reduce_f5_up(fc, j, evaluate, &hc_dat_local, &sc_wrapper);

          /* decompose into exterior loop part followed by a stem */
          en    = decompose_f5_ext_stem_d2(fc, j, evaluate, &hc_dat_local, &sc_wrapper);
          f5[j] = MIN2(f5[j], en);

          if (with_gquad) {
            en    = add_f5_gquad(fc, j, evaluate, &hc_dat_local, &sc_wrapper);
            f5[j] = MIN2(f5[j], en);
          }

          if ((grammar) && (grammar->cb_aux_f)) {
            en = grammar->cb_aux_f(fc, 1, j, grammar->auxdata);
            f5[j] = MIN2(f5[j], en);
          }
        }
        break;

      case 0:
        for (j = turn + 2; j <= length; j++) {
          /* extend previous solution(s) by adding an unpaired region */
          f5[j] = reduce_f5_up(fc, j, evaluate, &hc_dat_local, &sc_wrapper);

          /* decompose into exterior loop part followed by a stem */
          en    = decompose_f5_ext_stem_d0(fc, j, evaluate, &hc_dat_local, &sc_wrapper);
          f5[j] = MIN2(f5[j], en);

          if (with_gquad) {
            en    = add_f5_gquad(fc, j, evaluate, &hc_dat_local, &sc_wrapper);
            f5[j] = MIN2(f5[j], en);
          }

          if ((grammar) && (grammar->cb_aux_f)) {
            en = grammar->cb_aux_f(fc, 1, j, grammar->auxdata);
            f5[j] = MIN2(f5[j], en);
          }
        }
        break;

      default:
        for (j = turn + 2; j <= length; j++) {
          /* extend previous solution(s) by adding an unpaired region */
          f5[j] = reduce_f5_up(fc, j, evaluate, &hc_dat_local, &sc_wrapper);

          en    = decompose_f5_ext_stem_d1(fc, j, evaluate, &hc_dat_local, &sc_wrapper);
          f5[j] = MIN2(f5[j], en);

          if (with_gquad) {
            en    = add_f5_gquad(fc, j, evaluate, &hc_dat_local, &sc_wrapper);
            f5[j] = MIN2(f5[j], en);
          }

          if ((grammar) && (grammar->cb_aux_f)) {
            en = grammar->cb_aux_f(fc, 1, j, grammar->auxdata);
            f5[j] = MIN2(f5[j], en);
          }
        }
        break;
    }

    free_sc_wrapper(&sc_wrapper);

    return f5[length];
  }

  return INF;
}


PUBLIC int
vrna_E_ext_loop_3(vrna_fold_compound_t  *fc,
                  int                   i)
{
  if (fc) {
    int                       e, en, dangle_model, with_gquad;
    vrna_param_t              *P;
    vrna_md_t                 *md;
    vrna_callback_hc_evaluate *evaluate;
    struct default_data       hc_dat_local;
    struct sc_wrapper_f3      sc_wrapper;

    e = INF;

    P             = fc->params;
    md            = &(P->model_details);
    dangle_model  = md->dangles;
    with_gquad    = md->gquad;
    evaluate      = prepare_hc_default_window(fc, &hc_dat_local);

    init_sc_wrapper_f3(fc, i, &sc_wrapper);

    /* first case: i stays unpaired */
    e = reduce_f3_up(fc, i, evaluate, &hc_dat_local, &sc_wrapper);

    /* decompose into stem followed by exterior loop part */
    switch (dangle_model) {
      case 0:
        en  = decompose_f3_ext_stem_d0(fc, i, evaluate, &hc_dat_local, &sc_wrapper);
        e   = MIN2(e, en);
        break;

      case 2:
        en  = decompose_f3_ext_stem_d2(fc, i, evaluate, &hc_dat_local, &sc_wrapper);
        e   = MIN2(e, en);
        break;

      default:
        en  = decompose_f3_ext_stem_d1(fc, i, evaluate, &hc_dat_local, &sc_wrapper);
        e   = MIN2(e, en);
        break;
    }

    if (with_gquad) {
      en  = add_f3_gquad(fc, i, evaluate, &hc_dat_local, &sc_wrapper);
      e   = MIN2(e, en);
    }

    free_sc_wrapper_f3(&sc_wrapper);

    return e;
  }

  return INF;
}


PUBLIC int
vrna_E_ext_stem(unsigned int  type,
                int           n5d,
                int           n3d,
                vrna_param_t  *p)
{
  int energy = 0;

  if (n5d >= 0 && n3d >= 0)
    energy += p->mismatchExt[type][n5d][n3d];
  else if (n5d >= 0)
    energy += p->dangle5[type][n5d];
  else if (n3d >= 0)
    energy += p->dangle3[type][n3d];

  if (type > 2)
    energy += p->TerminalAU;

  return energy;
}


PUBLIC int
vrna_E_ext_loop(vrna_fold_compound_t  *vc,
                int                   i,
                int                   j)
{
  char                      *ptype;
  short                     *S;
  unsigned int              type;
  int                       ij, en, e, *idx;
  vrna_param_t              *P;
  vrna_md_t                 *md;
  vrna_sc_t                 *sc;
  vrna_callback_hc_evaluate *evaluate;
  struct default_data       hc_dat_local;

  S         = vc->sequence_encoding;
  idx       = vc->jindx;
  ptype     = vc->ptype;
  P         = vc->params;
  md        = &(P->model_details);
  sc        = vc->sc;
  evaluate  = prepare_hc_default(vc, &hc_dat_local);

  e     = INF;
  ij    = idx[j] + i;
  type  = vrna_get_ptype(ij, ptype);

  if (evaluate(i, j, i, j, VRNA_DECOMP_EXT_STEM, &hc_dat_local)) {
    switch (md->dangles) {
      case 2:
        e = E_ExtLoop(type, S[i - 1], S[j + 1], P);
        break;

      case 0:
      /* fall through */

      default:
        e = E_ExtLoop(type, -1, -1, P);
        break;
    }
    if (sc)
      if (sc->f)
        e += sc->f(i, j, i, j, VRNA_DECOMP_EXT_STEM, sc->data);
  }

  if (md->dangles % 2) {
    if (evaluate(i, j, i, j - 1, VRNA_DECOMP_EXT_STEM, &hc_dat_local)) {
      type = vrna_get_ptype(ij, ptype);

      en = E_ExtLoop(type, -1, S[j], P);

      if (sc)
        if (sc->f)
          en += sc->f(i, j, i, j - 1, VRNA_DECOMP_EXT_STEM, sc->data);

      e = MIN2(e, en);
    }

    ij = idx[j] + i + 1;
    if (evaluate(i, j, i + 1, j, VRNA_DECOMP_EXT_STEM, &hc_dat_local)) {
      type = vrna_get_ptype(ij, ptype);

      en = E_ExtLoop(type, S[i], -1, P);

      if (sc)
        if (sc->f)
          en += sc->f(i, j, i + 1, j, VRNA_DECOMP_EXT_STEM, sc->data);

      e = MIN2(e, en);
    }
  }

  return e;
}


/*
 #####################################
 # BEGIN OF STATIC HELPER FUNCTIONS  #
 #####################################
 */
PRIVATE INLINE int
decompose_f5_ext_stem_d0(vrna_fold_compound_t       *vc,
                         int                        j,
                         vrna_callback_hc_evaluate  *evaluate,
                         struct default_data        *hc_dat_local,
                         struct sc_wrapper_f5       *sc_wrapper)
{
  int e, *stems;

  stems = get_stem_contributions_d0(vc, j, evaluate, hc_dat_local, sc_wrapper);

  /* 1st case, actual decompostion */
  e = decompose_f5_ext_stem(vc, j, stems);

  /* 2nd case, reduce to single stem */
  e = MIN2(e, stems[1]);

  free(stems);

  return e;
}


PRIVATE INLINE int
decompose_f3_ext_stem_d0(vrna_fold_compound_t       *vc,
                         int                        i,
                         vrna_callback_hc_evaluate  *evaluate,
                         struct default_data        *hc_dat_local,
                         struct sc_wrapper_f3       *sc_wrapper)
{
  int e, *stems, maxdist, length;

  length  = (int)vc->length;
  maxdist = vc->window_size;

  stems = f3_get_stem_contributions_d0(vc, i, evaluate, hc_dat_local, sc_wrapper);

  /* 1st case, actual decompostion */
  e = decompose_f3_ext_stem(vc, i, MIN2(length - 1, i + maxdist), stems);

  /* 2nd case, reduce to single stem */
  if (length <= i + maxdist)
    e = MIN2(e, stems[length]);

  stems += i;
  free(stems);

  return e;
}


PRIVATE INLINE int
decompose_f5_ext_stem_d2(vrna_fold_compound_t       *vc,
                         int                        j,
                         vrna_callback_hc_evaluate  *evaluate,
                         struct default_data        *hc_dat_local,
                         struct sc_wrapper_f5       *sc_wrapper)
{
  int e, *stems;

  stems = get_stem_contributions_d2(vc, j, evaluate, hc_dat_local, sc_wrapper);

  /* 1st case, actual decompostion */
  e = decompose_f5_ext_stem(vc, j, stems);

  /* 2nd case, reduce to single stem */
  e = MIN2(e, stems[1]);

  free(stems);

  return e;
}


PRIVATE INLINE int
decompose_f3_ext_stem_d2(vrna_fold_compound_t       *vc,
                         int                        i,
                         vrna_callback_hc_evaluate  *evaluate,
                         struct default_data        *hc_dat_local,
                         struct sc_wrapper_f3       *sc_wrapper)
{
  int e, *stems, maxdist, length;

  length  = (int)vc->length;
  maxdist = vc->window_size;
  stems   = f3_get_stem_contributions_d2(vc, i, evaluate, hc_dat_local, sc_wrapper);

  /* 1st case, actual decompostion */
  e = decompose_f3_ext_stem(vc, i, MIN2(length - 1, i + maxdist), stems);

  /* 2nd case, reduce to single stem */
  if (length <= i + maxdist)
    e = MIN2(e, stems[length]);

  stems += i;
  free(stems);

  return e;
}


PRIVATE INLINE int
decompose_f5_ext_stem_d1(vrna_fold_compound_t       *vc,
                         int                        j,
                         vrna_callback_hc_evaluate  *evaluate,
                         struct default_data        *hc_dat_local,
                         struct sc_wrapper_f5       *sc_wrapper)
{
  int e, ee, *stems;

  e = INF;

  /* A) without dangling end contributions */

  /* 1st case, actual decompostion */
  stems = get_stem_contributions_d0(vc, j, evaluate, hc_dat_local, sc_wrapper);

  ee = decompose_f5_ext_stem(vc, j, stems);

  /* 2nd case, reduce to single stem */
  ee = MIN2(ee, stems[1]);

  free(stems);

  e = MIN2(e, ee);

  /* B) with dangling end contribution on 5' side of stem */
  stems = f5_get_stem_contributions_d5(vc, j, evaluate, hc_dat_local, sc_wrapper);

  /* 1st case, actual decompostion */
  ee = decompose_f5_ext_stem(vc, j, stems);

  /* 2nd case, reduce to single stem */
  ee = MIN2(ee, stems[1]);

  free(stems);

  e = MIN2(e, ee);

  /* C) with dangling end contribution on 3' side of stem */
  stems = f5_get_stem_contributions_d3(vc, j, evaluate, hc_dat_local, sc_wrapper);

  /* 1st case, actual decompostion */
  ee = decompose_f5_ext_stem(vc, j, stems);

  /* 2nd case, reduce to single stem */
  ee = MIN2(ee, stems[1]);

  free(stems);

  e = MIN2(e, ee);

  /* D) with dangling end contribution on both sides of stem */
  stems = f5_get_stem_contributions_d53(vc, j, evaluate, hc_dat_local, sc_wrapper);

  /* 1st case, actual decompostion */
  ee = decompose_f5_ext_stem(vc, j, stems);

  /* 2nd case, reduce to single stem */
  ee = MIN2(ee, stems[1]);

  free(stems);

  e = MIN2(e, ee);

  return e;
}


PRIVATE INLINE int
decompose_f3_ext_stem_d1(vrna_fold_compound_t       *vc,
                         int                        i,
                         vrna_callback_hc_evaluate  *evaluate,
                         struct default_data        *hc_dat_local,
                         struct sc_wrapper_f3       *sc_wrapper)
{
  int length, e, ee, *stems, maxdist;

  length  = (int)vc->length;
  maxdist = vc->window_size;
  e       = INF;

  /* A) without dangling end contributions */

  /* 1st case, actual decompostion */
  stems = f3_get_stem_contributions_d0(vc, i, evaluate, hc_dat_local, sc_wrapper);

  ee = decompose_f3_ext_stem(vc, i, MIN2(length - 1, i + maxdist), stems);

  /* 2nd case, reduce to single stem */
  if (length <= i + maxdist)
    ee = MIN2(ee, stems[length]);

  stems += i;
  free(stems);

  e = MIN2(e, ee);

  /* B) with dangling end contribution on 3' side of stem */
  stems = f3_get_stem_contributions_d3(vc, i, evaluate, hc_dat_local, sc_wrapper);

  /* 1st case, actual decompostion */
  ee = decompose_f3_ext_stem(vc, i, MIN2(length - 1, i + maxdist + 1), stems);

  /* 2nd case, reduce to single stem */
  if (length <= i + maxdist)
    ee = MIN2(ee, stems[length]);

  stems += i;
  free(stems);

  e = MIN2(e, ee);

  /* C) with dangling end contribution on 5' side of stem */
  stems = f3_get_stem_contributions_d5(vc, i, evaluate, hc_dat_local, sc_wrapper);

  /* 1st case, actual decompostion */
  ee = decompose_f3_ext_stem(vc, i, MIN2(length - 1, i + maxdist + 1), stems);

  /* 2nd case, reduce to single stem */
  if (length <= i + maxdist)
    ee = MIN2(ee, stems[length]);

  stems += i;
  free(stems);

  e = MIN2(e, ee);

  /* D) with dangling end contribution on both sides of stem */
  stems = f3_get_stem_contributions_d53(vc, i, evaluate, hc_dat_local, sc_wrapper);

  /* 1st case, actual decompostion */
  ee = decompose_f3_ext_stem(vc, i, MIN2(length - 1, i + maxdist + 1), stems);

  /* 2nd case, reduce to single stem */
  if (length <= i + maxdist)
    ee = MIN2(ee, stems[length]);

  stems += i;
  free(stems);

  e = MIN2(e, ee);

  return e;
}


/*
 *  extend f5 by adding an unpaired nucleotide or an unstructured domain
 *  to the 3' end
 */
PRIVATE INLINE int
reduce_f5_up(vrna_fold_compound_t       *vc,
             int                        j,
             vrna_callback_hc_evaluate  *evaluate,
             struct default_data        *hc_dat_local,
             struct sc_wrapper_f5       *sc_wrapper)
{
  int                 u, k, e, en, *f5;
  vrna_ud_t           *domains_up;
  sc_f5_reduce_to_ext *sc_red_ext;

  f5          = vc->matrices->f5;
  domains_up  = vc->domains_up;
  e           = INF;

  sc_red_ext = sc_wrapper->red_ext;

  /* check for 3' extension with one unpaired nucleotide */
  if (f5[j - 1] != INF) {
    if (evaluate(1, j, 1, j - 1, VRNA_DECOMP_EXT_EXT, hc_dat_local)) {
      e = f5[j - 1];

      if (sc_red_ext)
        e += sc_red_ext(j, 1, j - 1, sc_wrapper);
    }
  }

  if ((domains_up) && (domains_up->energy_cb)) {
    for (k = 0; k < domains_up->uniq_motif_count; k++) {
      u = domains_up->uniq_motif_size[k];
      if ((j - u >= 0) && (f5[j - u] != INF)) {
        if (evaluate(1, j, 1, j - u, VRNA_DECOMP_EXT_EXT, hc_dat_local)) {
          en = f5[j - u] +
               domains_up->energy_cb(vc,
                                     j - u + 1,
                                     j,
                                     VRNA_UNSTRUCTURED_DOMAIN_EXT_LOOP | VRNA_UNSTRUCTURED_DOMAIN_MOTIF,
                                     domains_up->data);

          if (sc_red_ext)
            en += sc_red_ext(j, 1, j - u, sc_wrapper);

          e = MIN2(e, en);
        }
      }
    }
  }

  return e;
}


PRIVATE INLINE int
reduce_f3_up(vrna_fold_compound_t       *vc,
             int                        i,
             vrna_callback_hc_evaluate  *evaluate,
             struct default_data        *hc_dat_local,
             struct sc_wrapper_f3       *sc_wrapper)
{
  int                 u, k, length, e, en, *f3;
  vrna_ud_t           *domains_up;
  sc_f3_reduce_to_ext *sc_red_ext;

  length      = (int)vc->length;
  f3          = vc->matrices->f3_local;
  domains_up  = vc->domains_up;
  e           = INF;

  sc_red_ext = sc_wrapper->red_ext;

  /* check for 3' extension with one unpaired nucleotide */
  if (f3[i + 1] != INF) {
    if (evaluate(i, length, i + 1, length, VRNA_DECOMP_EXT_EXT, hc_dat_local)) {
      e = f3[i + 1];

      if (sc_red_ext)
        e += sc_red_ext(i, i + 1, length, sc_wrapper);
    }
  }

  if ((domains_up) && (domains_up->energy_cb)) {
    for (k = 0; k < domains_up->uniq_motif_count; k++) {
      u = domains_up->uniq_motif_size[k];
      if ((i + u - 1 <= length) && (f3[i + u] != INF)) {
        if (evaluate(i, length, i + u - 1, length, VRNA_DECOMP_EXT_EXT, hc_dat_local)) {
          en = f3[i + u] +
               domains_up->energy_cb(vc,
                                     i,
                                     i + u - 1,
                                     VRNA_UNSTRUCTURED_DOMAIN_EXT_LOOP | VRNA_UNSTRUCTURED_DOMAIN_MOTIF,
                                     domains_up->data);

          if (sc_red_ext)
            en += sc_red_ext(i, i + u, length, sc_wrapper);

          e = MIN2(e, en);
        }
      }
    }
  }

  return e;
}


PRIVATE INLINE int *
get_stem_contributions_d0(vrna_fold_compound_t      *vc,
                          int                       j,
                          vrna_callback_hc_evaluate *evaluate,
                          struct default_data       *hc_dat_local,
                          struct sc_wrapper_f5      *sc_wrapper)
{
  char                    *ptype;
  short                   **S;
  unsigned int            s, n_seq, type;
  int                     i, ij, *indx, turn, *c, *stems;
  vrna_param_t            *P;
  vrna_md_t               *md;

  sc_f5_split_in_ext_stem *sc_spl_stem;
  sc_f5_reduce_to_stem    *sc_red_stem;

  stems = (int *)vrna_alloc(sizeof(int) * j);

  P     = vc->params;
  md    = &(P->model_details);
  indx  = vc->jindx;
  c     = vc->matrices->c;
  turn  = md->min_loop_size;
  ij    = indx[j] + j - turn - 1;
  ptype = (vc->type == VRNA_FC_TYPE_SINGLE) ? vc->ptype : NULL;
  n_seq = (vc->type == VRNA_FC_TYPE_SINGLE) ? 1 : vc->n_seq;
  S     = (vc->type == VRNA_FC_TYPE_SINGLE) ? NULL : vc->S;

  sc_spl_stem = sc_wrapper->decomp_stem;
  sc_red_stem = sc_wrapper->red_stem;

  switch (vc->type) {
    case VRNA_FC_TYPE_SINGLE:
      for (i = j - turn - 1; i > 1; i--, ij--) {
        stems[i] = INF;

        if ((c[ij] != INF) &&
            (evaluate(1, j, i - 1, i, VRNA_DECOMP_EXT_EXT_STEM, hc_dat_local))) {
          stems[i]  = c[ij];
          type      = vrna_get_ptype(ij, ptype);
          stems[i]  += E_ExtLoop(type, -1, -1, P);
        }
      }
      break;

    case VRNA_FC_TYPE_COMPARATIVE:
      for (i = j - turn - 1; i > 1; i--, ij--) {
        stems[i] = INF;
        if ((c[ij] != INF) &&
            (evaluate(1, j, i - 1, i, VRNA_DECOMP_EXT_EXT_STEM, hc_dat_local))) {
          stems[i] = c[ij];

          for (s = 0; s < n_seq; s++) {
            type      = vrna_get_ptype_md(S[s][i], S[s][j], md);
            stems[i]  += E_ExtLoop(type, -1, -1, P);
          }
        }
      }
      break;
  }

  if (sc_spl_stem)
    for (i = j - turn - 1; i > 1; i--)
      if (stems[i] != INF)
        stems[i] += sc_spl_stem(j, i - 1, i, sc_wrapper);

  stems[1]  = INF;
  ij        = indx[j] + 1;

  if ((c[ij] != INF) &&
      (evaluate(1, j, 1, j, VRNA_DECOMP_EXT_STEM, hc_dat_local))) {
    stems[1] = c[ij];

    switch (vc->type) {
      case VRNA_FC_TYPE_SINGLE:
        type      = vrna_get_ptype(ij, ptype);
        stems[1]  += E_ExtLoop(type, -1, -1, P);
        break;

      case VRNA_FC_TYPE_COMPARATIVE:
        for (s = 0; s < n_seq; s++) {
          type      = vrna_get_ptype_md(S[s][1], S[s][j], md);
          stems[1]  += E_ExtLoop(type, -1, -1, P);
        }
        break;
    }

    if (sc_red_stem)
      stems[1] += sc_red_stem(j, 1, j, sc_wrapper);
  }

  return stems;
}


PRIVATE INLINE int *
f3_get_stem_contributions_d0(vrna_fold_compound_t       *vc,
                             int                        i,
                             vrna_callback_hc_evaluate  *evaluate,
                             struct default_data        *hc_dat_local,
                             struct sc_wrapper_f3       *sc_wrapper)
{
  char                    **ptype;
  short                   **S, *si;
  unsigned int            s, n_seq, type, length;
  int                     energy, j, max_j, turn, *c, *stems, maxdist;
  vrna_param_t            *P;
  vrna_md_t               *md;

  sc_f3_split_in_ext_stem *sc_spl_stem;
  sc_f3_reduce_to_stem    *sc_red_stem;

  length  = vc->length;
  maxdist = vc->window_size;
  P       = vc->params;
  md      = &(P->model_details);
  c       = vc->matrices->c_local[i];
  c       -= i;
  turn    = md->min_loop_size;
  si      = NULL;
  ptype   = (vc->type == VRNA_FC_TYPE_SINGLE) ? vc->ptype_local : NULL;
  n_seq   = (vc->type == VRNA_FC_TYPE_SINGLE) ? 1 : vc->n_seq;
  S       = (vc->type == VRNA_FC_TYPE_SINGLE) ? NULL : vc->S;

  stems = (int *)vrna_alloc(sizeof(int) * (maxdist + 6));
  stems -= i;

  sc_spl_stem = sc_wrapper->decomp_stem;
  sc_red_stem = sc_wrapper->red_stem;

  max_j = MIN2(length - 1, i + maxdist);

  switch (vc->type) {
    case VRNA_FC_TYPE_SINGLE:
      for (j = i + turn + 1; j <= max_j; j++) {
        stems[j] = INF;
        if ((c[j] != INF) &&
            (evaluate(i, length, j, j + 1, VRNA_DECOMP_EXT_STEM_EXT, hc_dat_local))) {
          type      = vrna_get_ptype_window(i, j, ptype);
          stems[j]  = c[j] +
                      E_ExtLoop(type, -1, -1, P);
        }
      }
      break;

    case VRNA_FC_TYPE_COMPARATIVE:
      si = (short *)vrna_alloc(sizeof(short) * n_seq);

      for (s = 0; s < n_seq; s++)
        si[s] = S[s][i];

      for (j = i + turn + 1; j <= max_j; j++) {
        stems[j] = INF;
        if ((c[j] != INF) &&
            (evaluate(i, length, j, j + 1, VRNA_DECOMP_EXT_STEM_EXT, hc_dat_local))) {
          energy = c[j];
          for (s = 0; s < n_seq; s++) {
            type    = vrna_get_ptype_md(si[s], S[s][j], md);
            energy  += E_ExtLoop(type, -1, -1, P);
          }
          stems[j] = energy;
        }
      }
      break;
  }


  if (sc_spl_stem)
    for (j = i + turn + 1; j <= max_j; j++)
      if (stems[j] != INF)
        stems[j] += sc_spl_stem(i, j, j + 1, sc_wrapper);

  if (length <= i + maxdist) {
    j         = length;
    stems[j]  = INF;

    if ((c[j] != INF) &&
        (evaluate(i, j, i, j, VRNA_DECOMP_EXT_STEM, hc_dat_local))) {
      energy = c[j];

      switch (vc->type) {
        case VRNA_FC_TYPE_SINGLE:
          type    = vrna_get_ptype_window(i, j, ptype);
          energy  += E_ExtLoop(type, -1, -1, P);

          break;

        case VRNA_FC_TYPE_COMPARATIVE:
          for (s = 0; s < n_seq; s++) {
            type    = vrna_get_ptype_md(si[s], S[s][j], md);
            energy  += E_ExtLoop(type, -1, -1, P);
          }

          break;
      }

      if (sc_red_stem)
        energy += sc_red_stem(i, i, j, sc_wrapper);

      stems[j] = energy;
    }
  } else {
    /*
     * make sure we do not take (i + maxdist + 1) into account when
     * decomposing for odd dangle models
     */
    stems[i + maxdist + 1] = INF;
  }

  free(si);

  return stems;
}


PRIVATE INLINE int *
get_stem_contributions_d2(vrna_fold_compound_t      *vc,
                          int                       j,
                          vrna_callback_hc_evaluate *evaluate,
                          struct default_data       *hc_dat_local,
                          struct sc_wrapper_f5      *sc_wrapper)
{
  char                    *ptype;
  short                   *S, sj1, *si1, **SS, **S5, **S3, *s3j, *sj;
  unsigned int            s, n_seq, **a2s, type;
  int                     n, i, ij, *indx, turn, *c, *stems, mm5;
  vrna_param_t            *P;
  vrna_md_t               *md;

  sc_f5_split_in_ext_stem *sc_spl_stem;
  sc_f5_reduce_to_stem    *sc_red_stem;

  stems = (int *)vrna_alloc(sizeof(int) * j);

  n     = (int)vc->length;
  P     = vc->params;
  md    = &(P->model_details);
  indx  = vc->jindx;
  c     = vc->matrices->c;
  turn  = md->min_loop_size;
  ij    = indx[j] + j - turn - 1;

  sc_spl_stem = sc_wrapper->decomp_stem;
  sc_red_stem = sc_wrapper->red_stem;

  switch (vc->type) {
    case VRNA_FC_TYPE_SINGLE:
      S     = vc->sequence_encoding;
      ptype = vc->ptype;
      si1   = S + j - turn - 2;
      sj1   = (j < n) ? S[j + 1] : -1;

      for (i = j - turn - 1; i > 1; i--, ij--, si1--) {
        stems[i] = INF;
        if ((c[ij] != INF) &&
            (evaluate(1, j, i - 1, i, VRNA_DECOMP_EXT_EXT_STEM, hc_dat_local))) {
          type      = vrna_get_ptype(ij, ptype);
          stems[i]  = c[ij] +
                      E_ExtLoop(type, *si1, sj1, P);
        }
      }

      if (sc_spl_stem)
        for (i = j - turn - 1; i > 1; i--)
          if (stems[i] != INF)
            stems[i] += sc_spl_stem(j, i - 1, i, sc_wrapper);

      stems[1]  = INF;
      ij        = indx[j] + 1;

      if ((c[ij] != INF) && (evaluate(1, j, 1, j, VRNA_DECOMP_EXT_STEM, hc_dat_local))) {
        type      = vrna_get_ptype(ij, ptype);
        stems[1]  = c[ij] +
                    E_ExtLoop(type, -1, sj1, P);

        if (sc_red_stem)
          stems[1] += sc_red_stem(j, 1, j, sc_wrapper);
      }

      break;

    case VRNA_FC_TYPE_COMPARATIVE:
      n_seq = vc->n_seq;
      SS    = vc->S;
      S5    = vc->S5;
      S3    = vc->S3;
      a2s   = vc->a2s;

      /* pre-compute S3[s][j - 1] */
      s3j = (short *)vrna_alloc(sizeof(short) * n_seq);
      sj  = (short *)vrna_alloc(sizeof(short) * n_seq);
      for (s = 0; s < n_seq; s++) {
        s3j[s]  = (a2s[s][j] < a2s[s][SS[0][0]]) ? S3[s][j] : -1;
        sj[s]   = SS[s][j];
      }

      for (i = j - turn - 1; i > 1; i--, ij--) {
        stems[i] = INF;
        if ((c[ij] != INF) &&
            (evaluate(1, j, i - 1, i, VRNA_DECOMP_EXT_EXT_STEM, hc_dat_local))) {
          stems[i] = c[ij];
          for (s = 0; s < n_seq; s++) {
            type      = vrna_get_ptype_md(SS[s][i], sj[s], md);
            mm5       = (a2s[s][i] > 1) ? S5[s][i] : -1;
            stems[i]  += E_ExtLoop(type, mm5, s3j[s], P);
          }
        }
      }

      if (sc_spl_stem)
        for (i = j - turn - 1; i > 1; i--)
          if (stems[i] != INF)
            stems[i] += sc_spl_stem(j, i - 1, i, sc_wrapper);

      stems[1]  = INF;
      ij        = indx[j] + 1;

      if ((c[ij] != INF) && (evaluate(1, j, 1, j, VRNA_DECOMP_EXT_STEM, hc_dat_local))) {
        stems[1] = c[ij];

        for (s = 0; s < n_seq; s++) {
          type      = vrna_get_ptype_md(SS[s][1], sj[s], md);
          stems[1]  += E_ExtLoop(type, -1, s3j[s], P);
        }

        if (sc_red_stem)
          stems[1] += sc_red_stem(j, 1, j, sc_wrapper);
      }

      free(s3j);
      free(sj);

      break;
  }

  return stems;
}


PRIVATE INLINE int *
f3_get_stem_contributions_d2(vrna_fold_compound_t       *vc,
                             int                        i,
                             vrna_callback_hc_evaluate  *evaluate,
                             struct default_data        *hc_dat_local,
                             struct sc_wrapper_f3       *sc_wrapper)
{
  char                    **ptype;
  short                   **S, **S5, **S3, *S1, si1, sj1, *s5i1, *si;
  unsigned int            s, n_seq, type, length, **a2s;
  int                     energy, j, max_j, turn, *c, *stems, maxdist;
  vrna_param_t            *P;
  vrna_md_t               *md;

  sc_f3_split_in_ext_stem *sc_spl_stem;
  sc_f3_reduce_to_stem    *sc_red_stem;

  length  = vc->length;
  maxdist = vc->window_size;
  P       = vc->params;
  md      = &(P->model_details);
  c       = vc->matrices->c_local[i];
  c       -= i;
  turn    = md->min_loop_size;

  stems = (int *)vrna_alloc(sizeof(int) * (maxdist + 6));
  stems -= i;

  sc_spl_stem = sc_wrapper->decomp_stem;
  sc_red_stem = sc_wrapper->red_stem;

  switch (vc->type) {
    case VRNA_FC_TYPE_SINGLE:
      ptype = vc->ptype_local;
      S1    = vc->sequence_encoding;
      si1   = (i > 1) ? S1[i - 1] : -1;
      max_j = MIN2((int)length - 1, i + maxdist);

      for (j = i + turn + 1; j <= max_j; j++) {
        stems[j] = INF;
        if ((c[j] != INF) &&
            (evaluate(i, length, j, j + 1, VRNA_DECOMP_EXT_STEM_EXT, hc_dat_local))) {
          type      = vrna_get_ptype_window(i, j, ptype);
          sj1       = S1[j + 1];
          stems[j]  = c[j] +
                      E_ExtLoop(type, si1, sj1, P);
        }
      }

      if (sc_spl_stem)
        for (j = i + turn + 1; j <= max_j; j++)
          if (stems[j] != INF)
            stems[j] += sc_spl_stem(i, j, j + 1, sc_wrapper);

      if (length <= (unsigned int)(i + maxdist)) {
        j         = (int)length;
        stems[j]  = INF;

        if ((c[j] != INF) && (evaluate(i, j, i, j, VRNA_DECOMP_EXT_STEM, hc_dat_local))) {
          type = vrna_get_ptype_window(i, j, ptype);

          stems[j] = c[j] +
                     E_ExtLoop(type, si1, -1, P);

          if (sc_red_stem)
            stems[j] += sc_red_stem(i, i, j, sc_wrapper);
        }
      }

      break;

    case VRNA_FC_TYPE_COMPARATIVE:
      n_seq = vc->n_seq;
      S     = vc->S;
      S5    = vc->S5;
      S3    = vc->S3;
      a2s   = vc->a2s;
      max_j = MIN2((int)length - 1, i + maxdist);

      s5i1  = (short *)vrna_alloc(sizeof(short) * n_seq);
      si    = (short *)vrna_alloc(sizeof(short) * n_seq);
      for (s = 0; s < n_seq; s++) {
        s5i1[s] = (a2s[s][i] > 1) ? S5[s][i] : -1;
        si[s]   = S[s][i];
      }

      for (j = i + turn + 1; j <= max_j; j++) {
        stems[j] = INF;
        if ((c[j] != INF) &&
            (evaluate(i, length, j, j + 1, VRNA_DECOMP_EXT_STEM_EXT, hc_dat_local))) {
          energy = c[j];
          for (s = 0; s < n_seq; s++) {
            type    = vrna_get_ptype_md(si[s], S[s][j], md);
            sj1     = (a2s[s][j] < a2s[s][S[0][0]]) ? S3[s][j] : -1;
            energy  += E_ExtLoop(type, s5i1[s], sj1, P);
          }
          stems[j] = energy;
        }
      }

      if (sc_spl_stem)
        for (j = i + turn + 1; j <= max_j; j++)
          if (stems[j] != INF)
            stems[j] += sc_spl_stem(i, j, j + 1, sc_wrapper);

      if (length <= (unsigned int)(i + maxdist)) {
        j         = (int)length;
        stems[j]  = INF;

        if ((c[j] != INF) && (evaluate(i, j, i, j, VRNA_DECOMP_EXT_STEM, hc_dat_local))) {
          energy = c[j];
          for (s = 0; s < n_seq; s++) {
            type    = vrna_get_ptype_md(si[s], S[s][j], md);
            energy  += E_ExtLoop(type, s5i1[s], -1, P);
          }

          if (sc_red_stem)
            energy += sc_red_stem(i, i, j, sc_wrapper);

          stems[j] = energy;
        }
      }

      free(s5i1);
      free(si);

      break;
  }

  return stems;
}


PRIVATE INLINE int *
f5_get_stem_contributions_d5(vrna_fold_compound_t       *vc,
                             int                        j,
                             vrna_callback_hc_evaluate  *evaluate,
                             struct default_data        *hc_dat_local,
                             struct sc_wrapper_f5       *sc_wrapper)
{
  char                    *ptype;
  short                   *S, *si1, **SS, **S5, *sj;
  unsigned int            s, n_seq, **a2s, type;
  int                     i, ij, *indx, turn, *c, *stems, mm5;
  vrna_param_t            *P;
  vrna_md_t               *md;

  sc_f5_split_in_ext_stem *sc_spl_stem;
  sc_f5_reduce_to_stem    *sc_red_stem;

  stems = (int *)vrna_alloc(sizeof(int) * j);

  P     = vc->params;
  md    = &(P->model_details);
  indx  = vc->jindx;
  c     = vc->matrices->c;
  turn  = md->min_loop_size;
  ij    = indx[j] + j - turn;

  sc_spl_stem = sc_wrapper->decomp_stem;
  sc_red_stem = sc_wrapper->red_stem;

  switch (vc->type) {
    case VRNA_FC_TYPE_SINGLE:
      S     = vc->sequence_encoding;
      ptype = vc->ptype;
      si1   = S + j - turn - 1;

      for (i = j - turn - 1; i > 1; i--, ij--, si1--) {
        stems[i] = INF;
        if ((c[ij] != INF) &&
            (evaluate(1, j, i - 1, i + 1, VRNA_DECOMP_EXT_EXT_STEM, hc_dat_local))) {
          type      = vrna_get_ptype(ij, ptype);
          stems[i]  = c[ij] +
                      E_ExtLoop(type, *si1, -1, P);
        }
      }

      if (sc_spl_stem)
        for (i = j - turn - 1; i > 1; i--)
          if (stems[i] != INF)
            stems[i] += sc_spl_stem(j, i - 1, i + 1, sc_wrapper);

      stems[1]  = INF;
      ij        = indx[j] + 2;

      if ((c[ij] != INF) && (evaluate(1, j, 2, j, VRNA_DECOMP_EXT_STEM, hc_dat_local))) {
        type      = vrna_get_ptype(ij, ptype);
        stems[1]  = c[ij] +
                    E_ExtLoop(type, S[1], -1, P);

        if (sc_red_stem)
          stems[1] += sc_red_stem(j, 2, j, sc_wrapper);
      }

      break;

    case VRNA_FC_TYPE_COMPARATIVE:
      n_seq = vc->n_seq;
      SS    = vc->S;
      S5    = vc->S5;
      a2s   = vc->a2s;

      sj = (short *)vrna_alloc(sizeof(short) * n_seq);
      for (s = 0; s < n_seq; s++)
        sj[s] = SS[s][j];

      for (i = j - turn - 1; i > 1; i--, ij--) {
        stems[i] = INF;
        if ((c[ij] != INF) &&
            (evaluate(1, j, i - 1, i + 1, VRNA_DECOMP_EXT_EXT_STEM, hc_dat_local))) {
          stems[i] = c[ij];
          for (s = 0; s < n_seq; s++) {
            type      = vrna_get_ptype_md(SS[s][i + 1], sj[s], md);
            mm5       = (a2s[s][i + 1] > 1) ? S5[s][i + 1] : -1;
            stems[i]  = E_ExtLoop(type, mm5, -1, P);
          }
        }
      }

      if (sc_spl_stem)
        for (i = j - turn - 1; i > 1; i--)
          if (stems[i] != INF)
            stems[i] += sc_spl_stem(j, i - 1, i + 1, sc_wrapper);

      stems[1]  = INF;
      ij        = indx[j] + 2;

      if ((c[ij] != INF) && (evaluate(1, j, 2, j, VRNA_DECOMP_EXT_STEM, hc_dat_local))) {
        stems[1] = c[ij];
        for (s = 0; s < n_seq; s++) {
          type      = vrna_get_ptype_md(SS[s][2], sj[s], md);
          mm5       = (a2s[s][2] > 1) ? S5[s][2] : -1;
          stems[i]  = E_ExtLoop(type, mm5, -1, P);
        }

        if (sc_red_stem)
          stems[1] += sc_red_stem(j, 2, j, sc_wrapper);
      }

      free(sj);

      break;
  }

  return stems;
}


PRIVATE INLINE int *
f3_get_stem_contributions_d3(vrna_fold_compound_t       *vc,
                             int                        i,
                             vrna_callback_hc_evaluate  *evaluate,
                             struct default_data        *hc_dat_local,
                             struct sc_wrapper_f3       *sc_wrapper)
{
  char                    **ptype;
  short                   *S1, **S, **S3, sj1, *si;
  unsigned int            s, n_seq, **a2s, type;
  int                     energy, j, max_j, turn, *c, *stems, length, maxdist;
  vrna_param_t            *P;
  vrna_md_t               *md;

  sc_f3_split_in_ext_stem *sc_spl_stem;
  sc_f3_reduce_to_stem    *sc_red_stem;

  length  = (int)vc->length;
  maxdist = vc->window_size;
  P       = vc->params;
  md      = &(P->model_details);
  c       = vc->matrices->c_local[i];
  c       -= i;
  turn    = md->min_loop_size;

  stems = (int *)vrna_alloc(sizeof(int) * (maxdist + 6));
  stems -= i;

  sc_spl_stem = sc_wrapper->decomp_stem;
  sc_red_stem = sc_wrapper->red_stem;

  switch (vc->type) {
    case VRNA_FC_TYPE_SINGLE:
      S1    = vc->sequence_encoding;
      ptype = vc->ptype_local;
      max_j = MIN2(length - 1, i + maxdist + 1);

      for (j = i + turn + 1; j <= max_j; j++) {
        stems[j] = INF;
        if ((c[j - 1] != INF) &&
            (evaluate(i, length, j - 1, j + 1, VRNA_DECOMP_EXT_STEM_EXT, hc_dat_local))) {
          type      = vrna_get_ptype_window(i, j - 1, ptype);
          stems[j]  = c[j - 1] +
                      E_ExtLoop(type, -1, S1[j], P);
        }
      }

      if (sc_spl_stem)
        for (j = i + turn + 1; j <= max_j; j++)
          if (stems[j] != INF)
            stems[j] += sc_spl_stem(i, j - 1, j + 1, sc_wrapper);

      if (length <= i + maxdist) {
        j = length;

        if ((c[j - 1] != INF) &&
            (evaluate(i, j, i, j - 1, VRNA_DECOMP_EXT_STEM, hc_dat_local))) {
          type      = vrna_get_ptype_window(i, j - 1, ptype);
          stems[j]  = c[j - 1] +
                      E_ExtLoop(type, -1, S1[j], P);

          if (sc_red_stem)
            stems[j] += sc_red_stem(i, i, j - 1, sc_wrapper);
        }
      }

      break;

    case VRNA_FC_TYPE_COMPARATIVE:
      n_seq = vc->n_seq;
      S     = vc->S;
      S3    = vc->S3;
      a2s   = vc->a2s;
      max_j = MIN2(length - 1, i + maxdist + 1);

      si = (short *)vrna_alloc(sizeof(short) * n_seq);
      for (s = 0; s < n_seq; s++)
        si[s] = S[s][i];

      for (j = i + turn + 1; j <= max_j; j++) {
        stems[j] = INF;
        if ((c[j - 1] != INF) &&
            (evaluate(i, length, j - 1, j + 1, VRNA_DECOMP_EXT_STEM_EXT, hc_dat_local))) {
          energy = c[j - 1];
          for (s = 0; s < n_seq; s++) {
            type    = vrna_get_ptype_md(si[s], S[s][j - 1], md);
            sj1     = (a2s[s][j - 1] < a2s[s][S[0][0]]) ? S3[s][j - 1] : -1;
            energy  += E_ExtLoop(type, -1, sj1, P);
          }
          stems[j] = energy;
        }
      }

      if (sc_spl_stem)
        for (j = i + turn + 1; j <= max_j; j++)
          if (stems[j] != INF)
            stems[j] += sc_spl_stem(i, j - 1, j + 1, sc_wrapper);

      if (length <= i + maxdist) {
        j = length;

        if ((c[j - 1] != INF) &&
            (evaluate(i, j, i, j - 1, VRNA_DECOMP_EXT_STEM, hc_dat_local))) {
          energy = c[j - 1];
          for (s = 0; s < n_seq; s++) {
            type    = vrna_get_ptype_md(si[s], S[s][j - 1], md);
            sj1     = (a2s[s][j - 1] < a2s[s][S[0][0]]) ? S3[s][j - 1] : -1;
            energy  += E_ExtLoop(type, -1, sj1, P);
          }

          if (sc_red_stem)
            energy += sc_red_stem(i, i, j - 1, sc_wrapper);

          stems[j] = energy;
        }
      }

      free(si);

      break;
  }

  return stems;
}


PRIVATE INLINE int *
f5_get_stem_contributions_d3(vrna_fold_compound_t       *vc,
                             int                        j,
                             vrna_callback_hc_evaluate  *evaluate,
                             struct default_data        *hc_dat_local,
                             struct sc_wrapper_f5       *sc_wrapper)
{
  char                    *ptype;
  short                   *S, sj1, **SS, **S3, *s3j1, *ssj1;
  unsigned int            s, n_seq, **a2s, type;
  int                     i, ij, *indx, turn, *c, *stems;
  vrna_param_t            *P;
  vrna_md_t               *md;

  sc_f5_split_in_ext_stem *sc_spl_stem;
  sc_f5_reduce_to_stem    *sc_red_stem;

  stems = (int *)vrna_alloc(sizeof(int) * j);

  P     = vc->params;
  md    = &(P->model_details);
  indx  = vc->jindx;
  c     = vc->matrices->c;
  turn  = P->model_details.min_loop_size;
  ij    = indx[j - 1] + j - turn - 1;

  sc_spl_stem = sc_wrapper->decomp_stem1;
  sc_red_stem = sc_wrapper->red_stem;

  switch (vc->type) {
    case VRNA_FC_TYPE_SINGLE:
      S     = vc->sequence_encoding;
      ptype = vc->ptype;
      sj1   = S[j];

      for (i = j - turn - 1; i > 1; i--, ij--) {
        stems[i] = INF;
        if ((c[ij] != INF) &&
            (evaluate(1, j, i - 1, i, VRNA_DECOMP_EXT_EXT_STEM1, hc_dat_local))) {
          type      = vrna_get_ptype(ij, ptype);
          stems[i]  = c[ij] +
                      E_ExtLoop(type, -1, sj1, P);
        }
      }

      if (sc_spl_stem)
        for (i = j - turn - 1; i > 1; i--)
          if (stems[i] != INF)
            stems[i] += sc_spl_stem(j, i - 1, i, sc_wrapper);

      stems[1]  = INF;
      ij        = indx[j - 1] + 1;

      if ((c[ij] != INF) && (evaluate(1, j, 1, j - 1, VRNA_DECOMP_EXT_STEM, hc_dat_local))) {
        type      = vrna_get_ptype(ij, ptype);
        stems[1]  = c[ij] +
                    E_ExtLoop(type, -1, sj1, P);

        if (sc_red_stem)
          stems[1] += sc_red_stem(j, 1, j - 1, sc_wrapper);
      }

      break;

    case VRNA_FC_TYPE_COMPARATIVE:
      n_seq = vc->n_seq;
      SS    = vc->S;
      S3    = vc->S3;
      a2s   = vc->a2s;

      /* pre-compute S3[s][j - 1] */
      s3j1  = (short *)vrna_alloc(sizeof(short) * n_seq);
      ssj1  = (short *)vrna_alloc(sizeof(short) * n_seq);
      for (s = 0; s < n_seq; s++) {
        s3j1[s] = (a2s[s][j - 1] < a2s[s][SS[0][0]]) ? S3[s][j - 1] : -1;
        ssj1[s] = SS[s][j - 1];
      }

      for (i = j - turn - 1; i > 1; i--, ij--) {
        stems[i] = INF;
        if ((c[ij] != INF) &&
            (evaluate(1, j, i - 1, i, VRNA_DECOMP_EXT_EXT_STEM1, hc_dat_local))) {
          stems[i] = c[ij];
          for (s = 0; s < n_seq; s++) {
            type      = vrna_get_ptype_md(SS[s][i], ssj1[s], md);
            stems[i]  += E_ExtLoop(type, -1, s3j1[s], P);
          }
        }
      }

      if (sc_spl_stem)
        for (i = j - turn - 1; i > 1; i--)
          if (stems[i] != INF)
            stems[i] += sc_spl_stem(j, i - 1, i, sc_wrapper);

      stems[1]  = INF;
      ij        = indx[j - 1] + 1;

      if ((c[ij] != INF) && (evaluate(1, j, 1, j - 1, VRNA_DECOMP_EXT_STEM, hc_dat_local))) {
        stems[1] = c[ij];

        for (s = 0; s < n_seq; s++) {
          type      = vrna_get_ptype_md(SS[s][1], ssj1[s], md);
          stems[1]  += E_ExtLoop(type, -1, s3j1[s], P);
        }

        if (sc_red_stem)
          stems[1] += sc_red_stem(j, 1, j - 1, sc_wrapper);
      }

      free(s3j1);
      free(ssj1);

      break;
  }

  return stems;
}


PRIVATE INLINE int *
f3_get_stem_contributions_d5(vrna_fold_compound_t       *vc,
                             int                        i,
                             vrna_callback_hc_evaluate  *evaluate,
                             struct default_data        *hc_dat_local,
                             struct sc_wrapper_f3       *sc_wrapper)
{
  char                    **ptype;
  short                   *S1, **S, **S5, *s5i1, si, *si1;
  unsigned int            s, n_seq, **a2s, type;
  int                     energy, j, max_j, turn, *c, *stems, length, maxdist;
  vrna_param_t            *P;
  vrna_md_t               *md;

  sc_f3_split_in_ext_stem *sc_spl_stem;
  sc_f3_reduce_to_stem    *sc_red_stem;

  length  = (int)vc->length;
  maxdist = vc->window_size;
  P       = vc->params;
  md      = &(P->model_details);
  c       = vc->matrices->c_local[i + 1];
  c       -= i + 1;
  turn    = P->model_details.min_loop_size;

  stems = (int *)vrna_alloc(sizeof(int) * (maxdist + 6));
  stems -= i;

  sc_spl_stem = sc_wrapper->decomp_stem1;
  sc_red_stem = sc_wrapper->red_stem;

  switch (vc->type) {
    case VRNA_FC_TYPE_SINGLE:
      S1    = vc->sequence_encoding;
      ptype = vc->ptype_local;
      si    = S1[i];
      max_j = MIN2(length - 1, i + maxdist + 1);

      for (j = i + turn + 1; j <= max_j; j++) {
        stems[j] = INF;
        if ((c[j] != INF) &&
            (evaluate(i, length, j, j + 1, VRNA_DECOMP_EXT_STEM_EXT1, hc_dat_local))) {
          type      = vrna_get_ptype_window(i + 1, j, ptype);
          stems[j]  = c[j] +
                      E_ExtLoop(type, si, -1, P);
        }
      }

      if (sc_spl_stem)
        for (j = i + turn + 1; j <= max_j; j++)
          if (stems[j] != INF)
            stems[j] += sc_spl_stem(i, j, j + 1, sc_wrapper);

      if (length <= i + maxdist) {
        j         = length;
        stems[j]  = INF;

        if ((c[j] != INF) &&
            (evaluate(i, j, i + 1, j, VRNA_DECOMP_EXT_STEM, hc_dat_local))) {
          type      = vrna_get_ptype_window(i + 1, j, ptype);
          stems[j]  = c[j] +
                      E_ExtLoop(type, si, -1, P);

          if (sc_red_stem)
            stems[j] += sc_red_stem(i, i + 1, j, sc_wrapper);
        }
      }

      break;

    case VRNA_FC_TYPE_COMPARATIVE:
      n_seq = vc->n_seq;
      S     = vc->S;
      S5    = vc->S5;
      a2s   = vc->a2s;
      max_j = MIN2(length - 1, i + maxdist + 1);

      /* pre-compute S5[s][i] */
      s5i1  = (short *)vrna_alloc(sizeof(short) * n_seq);
      si1   = (short *)vrna_alloc(sizeof(short) * n_seq);
      for (s = 0; s < n_seq; s++) {
        s5i1[s] = (a2s[s][i + 1] > 1) ? S5[s][i + 1] : -1;
        si1[s]  = S[s][i + 1];
      }

      for (j = i + turn + 1; j <= max_j; j++) {
        stems[j] = INF;
        if ((c[j] != INF) &&
            (evaluate(i, length, j, j + 1, VRNA_DECOMP_EXT_STEM_EXT1, hc_dat_local))) {
          energy = c[j];
          for (s = 0; s < n_seq; s++) {
            type    = vrna_get_ptype_md(si1[s], S[s][j], md);
            energy  += E_ExtLoop(type, s5i1[s], -1, P);
          }
          stems[j] = energy;
        }
      }

      if (sc_spl_stem)
        for (j = i + turn + 1; j <= max_j; j++)
          if (stems[j] != INF)
            stems[j] += sc_spl_stem(i, j, j + 1, sc_wrapper);

      if (length <= i + maxdist) {
        j         = length;
        stems[j]  = INF;

        if ((c[j] != INF) &&
            (evaluate(i, j, i + 1, j, VRNA_DECOMP_EXT_STEM, hc_dat_local))) {
          energy = c[j];
          for (s = 0; s < n_seq; s++) {
            type    = vrna_get_ptype_md(si1[s], S[s][j], md);
            energy  += E_ExtLoop(type, s5i1[s], -1, P);
          }

          if (sc_red_stem)
            energy += sc_red_stem(i, i + 1, j, sc_wrapper);

          stems[j] = energy;
        }
      }

      free(s5i1);
      free(si1);

      break;
  }

  return stems;
}


PRIVATE INLINE int *
f5_get_stem_contributions_d53(vrna_fold_compound_t      *vc,
                              int                       j,
                              vrna_callback_hc_evaluate *evaluate,
                              struct default_data       *hc_dat_local,
                              struct sc_wrapper_f5      *sc_wrapper)
{
  char                    *ptype;
  short                   *S, *si1, sj1, **SS, **S5, **S3, *s3j1, *ssj1;
  unsigned int            s, n_seq, **a2s, type;
  int                     i, ij, *indx, turn, *c, *stems;
  vrna_param_t            *P;
  vrna_md_t               *md;

  sc_f5_split_in_ext_stem *sc_spl_stem;
  sc_f5_reduce_to_stem    *sc_red_stem;

  stems = (int *)vrna_alloc(sizeof(int) * j);

  P     = vc->params;
  md    = &(P->model_details);
  indx  = vc->jindx;
  c     = vc->matrices->c;
  turn  = md->min_loop_size;
  ij    = indx[j - 1] + j - turn;

  sc_spl_stem = sc_wrapper->decomp_stem1;
  sc_red_stem = sc_wrapper->red_stem;

  switch (vc->type) {
    case VRNA_FC_TYPE_SINGLE:
      S     = vc->sequence_encoding;
      ptype = vc->ptype;
      sj1   = S[j];
      si1   = S + j - turn - 1;

      for (i = j - turn - 1; i > 1; i--, ij--, si1--) {
        stems[i] = INF;
        if ((c[ij] != INF) &&
            (evaluate(1, j, i - 1, i + 1, VRNA_DECOMP_EXT_EXT_STEM1, hc_dat_local))) {
          type      = vrna_get_ptype(ij, ptype);
          stems[i]  = c[ij] +
                      E_ExtLoop(type, *si1, sj1, P);
        }
      }

      if (sc_spl_stem)
        for (i = j - turn - 1; i > 1; i--)
          if (stems[i] != INF)
            stems[i] += sc_spl_stem(j, i - 1, i + 1, sc_wrapper);

      stems[1]  = INF;
      ij        = indx[j - 1] + 2;

      if ((c[ij] != INF) && (evaluate(1, j, 2, j - 1, VRNA_DECOMP_EXT_STEM, hc_dat_local))) {
        type      = vrna_get_ptype(ij, ptype);
        stems[1]  = c[ij] +
                    E_ExtLoop(type, S[1], sj1, P);

        if (sc_red_stem)
          stems[1] += sc_red_stem(j, 2, j - 1, sc_wrapper);
      }

      break;

    case VRNA_FC_TYPE_COMPARATIVE:
      n_seq = vc->n_seq;
      SS    = vc->S;
      S5    = vc->S5;
      S3    = vc->S3;
      a2s   = vc->a2s;

      /* pre-compute S3[s][j - 1] */
      s3j1  = (short *)vrna_alloc(sizeof(short) * n_seq);
      ssj1  = (short *)vrna_alloc(sizeof(short) * n_seq);
      for (s = 0; s < n_seq; s++) {
        s3j1[s] = (a2s[s][j - 1] < a2s[s][SS[0][0]]) ? S3[s][j - 1] : -1;
        ssj1[s] = SS[s][j - 1];
      }

      for (i = j - turn - 1; i > 1; i--, ij--, si1--) {
        stems[i] = INF;
        if ((c[ij] != INF) &&
            (evaluate(1, j, i - 1, i + 1, VRNA_DECOMP_EXT_EXT_STEM1, hc_dat_local))) {
          stems[i] = c[ij];
          for (s = 0; s < n_seq; s++) {
            type      = vrna_get_ptype_md(SS[s][i + 1], ssj1[s], md);
            stems[i]  += E_ExtLoop(type, (a2s[s][i + 1] > 1) ? S5[s][i + 1] : -1, s3j1[s], P);
          }
        }
      }

      if (sc_spl_stem)
        for (i = j - turn - 1; i > 1; i--)
          if (stems[i] != INF)
            stems[i] += sc_spl_stem(j, i - 1, i + 1, sc_wrapper);

      stems[1]  = INF;
      ij        = indx[j - 1] + 2;

      if ((c[ij] != INF) && (evaluate(1, j, 2, j - 1, VRNA_DECOMP_EXT_STEM, hc_dat_local))) {
        stems[1] = c[ij];
        for (s = 0; s < n_seq; s++) {
          type      = vrna_get_ptype_md(SS[s][2], ssj1[s], md);
          stems[1]  += E_ExtLoop(type, (a2s[s][2] > 1) ? S5[s][2] : -1, s3j1[s], P);
        }

        if (sc_red_stem)
          stems[1] += sc_red_stem(j, 2, j - 1, sc_wrapper);
      }

      free(s3j1);
      free(ssj1);

      break;
  }

  return stems;
}


PRIVATE INLINE int *
f3_get_stem_contributions_d53(vrna_fold_compound_t      *vc,
                              int                       i,
                              vrna_callback_hc_evaluate *evaluate,
                              struct default_data       *hc_dat_local,
                              struct sc_wrapper_f3      *sc_wrapper)
{
  char                    **ptype;
  short                   *S1, **S, **S5, **S3, *s5i1, si1, sj1, *ssi1;
  unsigned int            s, n_seq, **a2s, type;
  int                     energy, j, max_j, turn, *c, *stems, length, maxdist;
  vrna_param_t            *P;
  vrna_md_t               *md;

  sc_f3_split_in_ext_stem *sc_spl_stem;
  sc_f3_reduce_to_stem    *sc_red_stem;

  length  = (int)vc->length;
  maxdist = vc->window_size;
  P       = vc->params;
  md      = &(P->model_details);
  c       = vc->matrices->c_local[i + 1];
  c       -= i + 1;
  turn    = md->min_loop_size;

  stems = (int *)vrna_alloc(sizeof(int) * (maxdist + 6));
  stems -= i;

  sc_spl_stem = sc_wrapper->decomp_stem1;
  sc_red_stem = sc_wrapper->red_stem;

  switch (vc->type) {
    case VRNA_FC_TYPE_SINGLE:
      S1    = vc->sequence_encoding;
      ptype = vc->ptype_local;
      si1   = S1[i];
      max_j = MIN2(length - 1, i + maxdist + 1);

      for (j = i + turn + 1; j <= max_j; j++) {
        stems[j] = INF;
        if ((c[j - 1] != INF) &&
            (evaluate(i, length, j - 1, j + 1, VRNA_DECOMP_EXT_STEM_EXT1, hc_dat_local))) {
          type      = vrna_get_ptype_window(i + 1, j - 1, ptype);
          stems[j]  = c[j - 1] +
                      E_ExtLoop(type, si1, S1[j], P);
        }
      }

      if (sc_spl_stem)
        for (j = i + turn + 1; j <= max_j; j++)
          if (stems[j] != INF)
            stems[j] += sc_spl_stem(i, j - 1, j + 1, sc_wrapper);

      if (length <= i + maxdist) {
        j = length;
        if ((c[j - 1] != INF) &&
            (evaluate(i, length, i + 1, j - 1, VRNA_DECOMP_EXT_STEM, hc_dat_local))) {
          type      = vrna_get_ptype_window(i + 1, j - 1, ptype);
          stems[j]  = c[j - 1] +
                      E_ExtLoop(type, si1, S1[j], P);

          if (sc_red_stem)
            stems[j] += sc_red_stem(i, i + 1, j - 1, sc_wrapper);
        }
      }

      break;

    case VRNA_FC_TYPE_COMPARATIVE:
      n_seq = vc->n_seq;
      S     = vc->S;
      S5    = vc->S5;
      S3    = vc->S3;
      a2s   = vc->a2s;
      max_j = MIN2(length - 1, i + maxdist + 1);

      s5i1  = (short *)vrna_alloc(sizeof(short) * n_seq);
      ssi1  = (short *)vrna_alloc(sizeof(short) * n_seq);
      for (s = 0; s < n_seq; s++) {
        s5i1[s] = (a2s[s][i + 1] > 1) ? S5[s][i + 1] : -1;
        ssi1[s] = S[s][i + 1];
      }

      for (j = i + turn + 1; j <= max_j; j++) {
        stems[j] = INF;
        if ((c[j - 1] != INF) &&
            (evaluate(i, length, j - 1, j + 1, VRNA_DECOMP_EXT_STEM_EXT1, hc_dat_local))) {
          energy = c[j - 1];
          for (s = 0; s < n_seq; s++) {
            type    = vrna_get_ptype_md(ssi1[s], S[s][j - 1], md);
            sj1     = (a2s[s][j - 1] < a2s[s][S[0][0]]) ? S3[s][j - 1] : -1;
            energy  += E_ExtLoop(type, s5i1[s], sj1, P);
          }
          stems[j] = energy;
        }
      }

      if (sc_spl_stem)
        for (j = i + turn + 1; j <= max_j; j++)
          if (stems[j] != INF)
            stems[j] += sc_spl_stem(i, j - 1, j + 1, sc_wrapper);

      if (length <= i + maxdist) {
        j = length;
        if ((c[j - 1] != INF) &&
            (evaluate(i, length, i + 1, j - 1, VRNA_DECOMP_EXT_STEM, hc_dat_local))) {
          energy = c[j - 1];
          for (s = 0; s < n_seq; s++) {
            type    = vrna_get_ptype_md(ssi1[s], S[s][j - 1], md);
            sj1     = (a2s[s][j - 1] < a2s[s][S[0][0]]) ? S3[s][j - 1] : -1;
            energy  += E_ExtLoop(type, s5i1[s], sj1, P);
          }

          if (sc_red_stem)
            energy += sc_red_stem(i, i + 1, j - 1, sc_wrapper);

          stems[j] = energy;
        }
      }

      free(s5i1);
      free(ssi1);

      break;
  }

  return stems;
}


PRIVATE INLINE int
add_f5_gquad(vrna_fold_compound_t       *vc,
             int                        j,
             vrna_callback_hc_evaluate  *evaluate,
             struct default_data        *hc_dat_local,
             struct sc_wrapper_f5       *sc_wrapper)
{
  int e, i, ij, *indx, turn, *f5, *ggg;

  indx  = vc->jindx;
  f5    = vc->matrices->f5;
  ggg   = vc->matrices->ggg;
  turn  = vc->params->model_details.min_loop_size;
  ij    = indx[j] + j - turn - 1;
  e     = INF;

  for (i = j - turn - 1; i > 1; i--, ij--)
    if ((f5[i - 1] != INF) && (ggg[ij] != INF))
      e = MIN2(e, f5[i - 1] + ggg[ij]);

  ij  = indx[j] + 1;
  e   = MIN2(e, ggg[ij]);

  return e;
}


PRIVATE INLINE int
add_f3_gquad(vrna_fold_compound_t       *vc,
             int                        i,
             vrna_callback_hc_evaluate  *evaluate,
             struct default_data        *hc_dat_local,
             struct sc_wrapper_f3       *sc_wrapper)
{
  int e, j, length, turn, *f3, *ggg, maxdist;

  length  = (int)vc->length;
  maxdist = vc->window_size;
  f3      = vc->matrices->f3_local;
  ggg     = vc->matrices->ggg_local[i];
  turn    = vc->params->model_details.min_loop_size;
  e       = INF;

  for (j = i + turn + 1; (j < length) && (j <= i + maxdist); j++)
    if ((f3[j + 1] != INF) && (ggg[j - i] != INF))
      e = MIN2(e, f3[j + 1] + ggg[j - i]);

  if (length <= i + maxdist)
    e = MIN2(e, ggg[length - i]);

  return e;
}


#ifdef VRNA_WITH_SSE_IMPLEMENTATION
/* SSE modular decomposition -------------------------------*/
#include <emmintrin.h>
#include <smmintrin.h>

PRIVATE INLINE int
horizontal_min_Vec4i(__m128i x)
{
  __m128i min1  = _mm_shuffle_epi32(x, _MM_SHUFFLE(0, 0, 3, 2));
  __m128i min2  = _mm_min_epi32(x, min1);
  __m128i min3  = _mm_shuffle_epi32(min2, _MM_SHUFFLE(0, 0, 0, 1));
  __m128i min4  = _mm_min_epi32(min2, min3);

  return _mm_cvtsi128_si32(min4);
}


#endif


PRIVATE INLINE int
decompose_f5_ext_stem(vrna_fold_compound_t  *vc,
                      int                   j,
                      int                   *stems)
{
  int e, i, *f5, turn;

  f5    = vc->matrices->f5;
  turn  = vc->params->model_details.min_loop_size;
  e     = INF;

  /* modular decomposition */
#if VRNA_WITH_SSE_IMPLEMENTATION
  __m128i   inf = _mm_set1_epi32(INF);

  const int end = j - turn;

  for (i = 2; i < end - 3; i += 4) {
    __m128i   a     = _mm_loadu_si128((__m128i *)&f5[i - 1]);
    __m128i   b     = _mm_loadu_si128((__m128i *)&stems[i]);
    __m128i   c     = _mm_add_epi32(a, b);
    __m128i   mask1 = _mm_cmplt_epi32(a, inf);
    __m128i   mask2 = _mm_cmplt_epi32(b, inf);
    __m128i   res   = _mm_or_si128(_mm_and_si128(mask1, c),
                                   _mm_andnot_si128(mask1, a));

    res = _mm_or_si128(_mm_and_si128(mask2, res),
                       _mm_andnot_si128(mask2, b));
    const int en = horizontal_min_Vec4i(res);
    e = MIN2(e, en);
  }

  for (; i < end; i++) {
    if ((f5[i - 1] != INF) && (stems[i] != INF)) {
      const int en = f5[i - 1] + stems[i];
      e = MIN2(e, en);
    }
  }
#else
  for (i = 2; i < j - turn; i++)
    if ((f5[i - 1] != INF) && (stems[i] != INF)) {
      const int en = f5[i - 1] + stems[i];
      e = MIN2(e, en);
    }

#endif

  return e;
}


PRIVATE INLINE int
decompose_f3_ext_stem(vrna_fold_compound_t  *vc,
                      int                   i,
                      int                   max_j,
                      int                   *stems)
{
  int e, j, *f3, turn;

  f3    = vc->matrices->f3_local;
  turn  = vc->params->model_details.min_loop_size;
  e     = INF;

  /* modular decomposition */
#if VRNA_WITH_SSE_IMPLEMENTATION
  __m128i inf = _mm_set1_epi32(INF);

  for (j = i + turn + 1; j < max_j - 3; j += 4) {
    __m128i   a     = _mm_loadu_si128((__m128i *)&f3[j + 1]);
    __m128i   b     = _mm_loadu_si128((__m128i *)&stems[j]);
    __m128i   c     = _mm_add_epi32(a, b);
    __m128i   mask1 = _mm_cmplt_epi32(a, inf);
    __m128i   mask2 = _mm_cmplt_epi32(b, inf);
    __m128i   res   = _mm_or_si128(_mm_and_si128(mask1, c),
                                   _mm_andnot_si128(mask1, a));

    res = _mm_or_si128(_mm_and_si128(mask2, res),
                       _mm_andnot_si128(mask2, b));
    const int en = horizontal_min_Vec4i(res);
    e = MIN2(e, en);
  }

  for (; j <= max_j; j++) {
    if ((f3[j + 1] != INF) && (stems[j] != INF)) {
      const int en = f3[j + 1] + stems[j];
      e = MIN2(e, en);
    }
  }
#else
  for (j = i + turn + 1; j <= max_j; j++)
    if ((f3[j + 1] != INF) && (stems[j] != INF)) {
      const int en = stems[j] + f3[j + 1];
      e = MIN2(e, en);
    }

#endif

  return e;
}


/*###########################################*/
/*# deprecated functions below              #*/
/*###########################################*/

#ifndef VRNA_DISABLE_BACKWARD_COMPATIBILITY

PUBLIC int
E_Stem(int          type,
       int          si1,
       int          sj1,
       int          extLoop,
       vrna_param_t *P)
{
  int energy  = 0;
  int d5      = (si1 >= 0) ? P->dangle5[type][si1] : 0;
  int d3      = (sj1 >= 0) ? P->dangle3[type][sj1] : 0;

  if (type > 2)
    energy += P->TerminalAU;

  if (si1 >= 0 && sj1 >= 0)
    energy += (extLoop) ? P->mismatchExt[type][si1][sj1] : P->mismatchM[type][si1][sj1];
  else
    energy += d5 + d3;

  if (!extLoop)
    energy += P->MLintern[type];

  return energy;
}


PUBLIC int
E_ExtLoop(int           type,
          int           si1,
          int           sj1,
          vrna_param_t  *P)
{
  int energy = 0;

  if (si1 >= 0 && sj1 >= 0)
    energy += P->mismatchExt[type][si1][sj1];
  else if (si1 >= 0)
    energy += P->dangle5[type][si1];
  else if (sj1 >= 0)
    energy += P->dangle3[type][sj1];

  if (type > 2)
    energy += P->TerminalAU;

  return energy;
}


#endif
