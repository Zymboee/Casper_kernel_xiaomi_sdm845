/* bbr3_compat.h - Kernel 4.9 compatibility shims for BBRv3 */
#ifndef _BBR3_COMPAT_H
#define _BBR3_COMPAT_H

/* === cwnd accessors (4.9 uses direct field, 6.x uses accessor) === */
#define tcp_snd_cwnd(tp)           ((tp)->snd_cwnd)
#define tcp_snd_cwnd_set(tp, val)  ((tp)->snd_cwnd = (val))

/* === BPF kfunc annotation (doesn't exist in 4.9) === */
#define __bpf_kfunc

/* === GSO_LEGACY_MAX_SIZE (doesn't exist in 4.9) === */
#define GSO_LEGACY_MAX_SIZE        65536

/* === Random number API (4.9 uses prandom_u32_max) === */
#define get_random_u32_below(x)    prandom_u32_max(x)

/* === sk_pacing_shift (doesn't exist in 4.9, default is 10) === */
#define sk_pacing_shift_compat     10

/* === Pacing status (doesn't exist in 4.9) === */
#define SK_PACING_NONE  0
#define SK_PACING_NEEDED 1

/* === ECN flags (don't exist in 4.9) === */
#ifndef TCP_ECN_LOW
#define TCP_ECN_LOW             16
#endif
#ifndef TCP_ECN_ECT_PERMANENT
#define TCP_ECN_ECT_PERMANENT   32
#endif

/* === TCP_CONG_WANTS_CE_EVENTS (doesn't exist in 4.9) === */
#ifndef TCP_CONG_WANTS_CE_EVENTS
#define TCP_CONG_WANTS_CE_EVENTS  0x4
#endif

/* === CA_EVENT_TLP_RECOVERY (doesn't exist in 4.9) === */
#ifndef CA_EVENT_TLP_RECOVERY
#define CA_EVENT_TLP_RECOVERY  7
#endif

/* === PLB stubs (entire subsystem missing in 4.9) === */
struct tcp_plb_state {
    u8  consec_cong_rounds:5,
        unused:3;
    u32 pause_until;
} __attribute__((__packed__));

#define TCP_PLB_SCALE  8

static inline void tcp_plb_init(const struct sock *sk,
                                struct tcp_plb_state *plb) {}
static inline void tcp_plb_update_state(const struct sock *sk,
                                        struct tcp_plb_state *plb,
                                        int ce_ratio) {}
static inline void tcp_plb_check_rehash(struct sock *sk,
                                        struct tcp_plb_state *plb) {}
static inline void tcp_plb_update_state_upon_rto(struct sock *sk,
                                                 struct tcp_plb_state *plb) {}

/* === DCTCP ECE ACK helper stub (tcp_dctcp.h doesn't exist in 4.9) === */
static inline void dctcp_ece_ack_update(struct sock *sk,
                                        enum tcp_ca_event event,
                                        u32 *prior_rcv_nxt,
                                        u32 *ce_state) {}

/* === RTAX_FEATURE_ECN_LOW (route feature, missing in 4.9) === */
#ifndef RTAX_FEATURE_ECN_LOW
#define RTAX_FEATURE_ECN_LOW  (1 << 4)
#endif

/* === tcp_set_ecn_low_from_dst (doesn't exist in 4.9) === */
static inline void tcp_set_ecn_low_from_dst(struct sock *sk,
                                            const struct dst_entry *dst) {}

/* === tcp_stamp_us_delta shim (since we're using .v64 directly) === */
static inline s32 tcp_stamp_us_delta(u64 t1, u64 t0)
{
	struct skb_mstamp mt1, mt0;
	mt1.v64 = t1;
	mt0.v64 = t0;
	return skb_mstamp_us_delta(&mt1, &mt0);
}

#endif /* _BBR3_COMPAT_H */
