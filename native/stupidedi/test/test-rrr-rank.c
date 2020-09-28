#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include "stupidedi/include/rrr.h"
#include "stupidedi/include/bitstr.h"

int
main(int argc, char **argv)
{
    stupidedi_bitstr_t b;
    stupidedi_rrr_t rrr;

    size_t length, width;
    width  = 8;
    length = 8;

    uint64_t value = 0xaaaaaaaaaaaaaaaa;
    value  = -1;
    value &= (1ULL << width) - 1;

    /*
     *           4         6         8        12        13        16
     *         <-0       <-1       <-2       <-3       <-4       <-5    MARKERS
     *  ================================================================~~~~~~~~
     *  0101010100000000010101010000000001010101000000000101010100000000
     *  ............................................................
     *   <0 <1 <2 <3 <4 <5 <6 <7 <8 <9 <0 <1 <2  3  4  5  6  7  8  9  0
     *                                  1  1  1  1  1  1  1  1  1  1  2
     *
     *        <-0      <-1      <-2      <-3      <-4      <-5      <-6
     *          4        5        8       10       12       15       16
     */

    stupidedi_bitstr_init(&b, length * width);
    for (int k = 0; k + width <= stupidedi_bitstr_length(&b); k += width)
        stupidedi_bitstr_write(&b, k, width, value);

    stupidedi_rrr_init(&rrr, &b, 13, 88);
    for (size_t k = 0; k <= stupidedi_bitstr_length(&b) + 3; ++k)
    {
        if (k < stupidedi_bitstr_length(&b))
            printf("%zu: %llu rank₁(%zu)=%zu rank₀(%zu)=%zu\n",
                    k, stupidedi_bitstr_read(&b, k, 1),
                    k, stupidedi_rrr_rank1(&rrr, k),
                    k, stupidedi_rrr_rank0(&rrr, k));
        else
            printf("%zu: ? rank₁(%zu)=%zu rank₀(%zu)=%zu\n",
                    k,
                    k, stupidedi_rrr_rank1(&rrr, k),
                    k, stupidedi_rrr_rank0(&rrr, k));
    }

    printf("\n%s\n", stupidedi_bitstr_to_string(&b));
    printf("\n%s\n", stupidedi_rrr_to_string(&rrr));

    /*
    class Array;def cumsum; s=0; self.map{|x| s+=x }; end;end
    xs = "01010101,00000000,01010101,00000000,01010101,00000000,01010101,00000000"
    bs = xs.gsub(",","").scan(/.{1,9}/)
    bx = bs.map{|b| Integer(b.reverse, 2) }

    # Classes, ranks
    cs = bs.map{|b| b.count("1") }
    rs = cs.cumsum

    # Widths
    ws = cs.map{|r| (r.zero? && 0) || Math.log2([0,9,36,84,126,84,36,9][r]).ceil }
    os = ws.cumsum

    # Markers
    mb = xs.gsub(",","").scan(/.{1,10}/)
    ms = mb.map{|b| b.count("1") }.cumsum
    */
}