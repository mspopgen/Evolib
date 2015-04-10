import sys

from evolib.iterators import vcf_iter

for row in vcf_iter(sys.stdin):

    chrom, pos = row['CHROM'], row['POS']
    hp = [r['HP'] for r in row.iter_samples()]
    print hp
