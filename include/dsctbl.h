#ifndef __DSCTBL_H__
#define __DSCTBL_H__

struct SEGMENT_DESCRIPTOR {
    short limit_low, base_low;
    char base_mid, access_right;
    char limit_high, base_high;
};

struct GATE_DESCRIPTOR {
    short offset_low, selector;
    char dw_count, access_right;
    short offset_high;
};

//GDT関係
void init_gdtidt(void);
void set_segmdesc(struct SEGMENT_DESCRIPTOR* sd, unsigned int limit, int base, int ar);
void set_gatedesc(struct GATE_DESCRIPTOR* gd, int offset, int selector, int ar);
//void load_gdtr(int limit, int addr);
//void load_idtr(int limit, int addr);
#endif