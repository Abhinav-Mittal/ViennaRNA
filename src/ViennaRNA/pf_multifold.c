#include <stdio.h>
#include <stdlib.h>

#include "ViennaRNA/utils/basic.h"
#include "ViennaRNA/alphabet.h"
#include "ViennaRNA/loops/external.h"
#include "ViennaRNA/pf_multifold.h"

#ifdef __GNUC__
# define INLINE inline
#else
# define INLINE
#endif

#include "ViennaRNA/loops/external_hc.inc"

PRIVATE FLT_OR_DBL
mf_rule_pair(vrna_fold_compound_t *fc,
             int                  i,
             int                  j,
             void                 *data);


/*
 #################################
 # BEGIN OF FUNCTION DEFINITIONS #
 #################################
 */
PUBLIC int
vrna_pf_multifold_prepare(vrna_fold_compound_t *fc)
{
  if (fc)
    return vrna_gr_set_aux_exp_c(fc, &mf_rule_pair);

  return 0;
}


/*
 #################################
 # STATIC helper functions below #
 #################################
 */
PRIVATE FLT_OR_DBL
mf_rule_pair(vrna_fold_compound_t *fc,
             int                  i,
             int                  j,
             void                 *data)
{
  short                     *S1, *S2;
  unsigned int              *sn, *ends, type, nick;
  int                       *my_iindx;
  FLT_OR_DBL                contribution, *q, qbase, tmp;
  vrna_exp_param_t          *pf_params;
  vrna_md_t                 *md;
  vrna_callback_hc_evaluate *evaluate;
  struct default_data       hc_dat_local;

  contribution  = 0;
  S1            = fc->sequence_encoding;
  S2            = fc->sequence_encoding2;
  pf_params     = fc->exp_params;
  md            = &(pf_params->model_details);
  sn            = fc->strand_number;
  ends          = fc->strand_end;
  q             = fc->exp_matrices->q;
  my_iindx      = fc->iindx;
  evaluate      = prepare_hc_default(fc, &hc_dat_local);

  if ((sn[i] != sn[j]) &&
      (evaluate(i, j, i, j, VRNA_DECOMP_EXT_STEM, &hc_dat_local))) {
    /* most obious strand nick is at end of sn[i] and start of sn[j] */
    type  = vrna_get_ptype_md(S2[j], S2[i], md);
    qbase = vrna_exp_E_ext_stem(type, S1[j - 1], S1[i + 1], pf_params);
    tmp   = 0.;

    if (evaluate(i + 1,
                 j - 1,
                 ends[sn[i]],
                 ends[sn[i]] + 1,
                 VRNA_DECOMP_EXT_EXT_EXT,
                 &hc_dat_local))
      tmp += q[my_iindx[i + 1] - ends[sn[i]]] *
             q[my_iindx[ends[sn[i]] + 1] - j + 1];

    /* check whether we find more strand nicks between i and j */
    nick = ends[sn[i]] + 1;
    while (sn[nick] != sn[j]) {
      if (evaluate(i + 1,
                   j - 1,
                   ends[sn[nick]],
                   ends[sn[nick]] + 1,
                   VRNA_DECOMP_EXT_EXT_EXT,
                   &hc_dat_local))
        tmp += q[my_iindx[i + 1] - ends[sn[nick]]] *
               q[my_iindx[ends[sn[nick]] + 1] - j + 1];

      nick = ends[sn[nick]] + 1;
    }

    contribution = qbase *
                   tmp;
  }

  return contribution;
}
