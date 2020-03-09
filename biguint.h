int biguint_add(uint64_t *dst, uint64_t *src1, uint64_t *src2, int nints)
{
    uint64_t mask = ((uint64_t) 1 << 63) - 1;
    uint64_t carry = 0;
    for (int i = 0; i < nints; i++) {
        uint64_t tmp = (src1[i] & mask) + (src2[i] & mask) + carry;
        carry = (src1[i] >> 63) + (src2[i] >> 63) + (tmp >> 63);
        dst[i] = (tmp & mask) + ((carry & 1) << 63);
        carry >>= 1;
    }
    return carry;
}
