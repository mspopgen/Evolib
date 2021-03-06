#import numpy

def block_iter(itr, size = 3, start = 1):
    """
    Iterator that is aware of all items within non-overlapping 
    blocks of length 'size'. Returns the current item value in 
    the iterator along with all other items within it's local 
    bin. This is useful for scenarios where you may wish to apply 
    a method to the current item in the iteration that is dependent 
    on other items in the near vacinity. One example would be when 
    calculating statistics based on whether a mutation is amino acid 
    changing or not. The statistic calculated for each site depends 
    on the contents of the codon in which it is located. For example:
    
        dna = iter(['A', 'T', 'G', 'C', 
                    'A', 'T', 'G', 'C', 
                    'A', 'T', 'G', 'C'])
                        
        for base in backToTheFutureIter(dna):
            print base[0], ''.join(base[1])
        
    Would give:
            
            A ATG
            T ATG
            G ATG
            C CAT
            A CAT
            T CAT
            G GCA
            C GCA
            A GCA
            T TGC
            G TGC
            C TGC
    """
    assert size > 1
    assert start > 0
    assert start <= size
    
    i = start
    if i != 1:
        p = []
        for k in range(start, size + 1):
            nxt = itr.next()
            p.append(nxt)
    
        while i <= size:
            yield (p[(i - (1 + start - 1)) % size], p)
            i += 1
        
    while True:
        
        if i % size == 1:
            p = []
            for j in range(size):
                try:
                    nxt = itr.next()
                    p.append(nxt)
                except StopIteration:
                    pass
       
        try:
            yield (p[(i - 1) % size], p)
        except IndexError:
            raise StopIteration
        i += 1


def create_ids(nids, prefix):
    ids = []
    for i in range(nseqs):
        seqnum = i + 1
        num_zeros = len(str(nseqs)) - len(str(seqnum))
        zeros = '0' * num_zeros
        id1 = prefix + zeros + str(seqnum)
        ids.append(id1)
            
    return ids


def loopByColumn(array, start = 0, size = 1):
    """
    m x n
      ['abcdef',
       'abcdef',
       'abcdef']
    
    n x m
      ['aaa', 'bbb', 'ccc',
       'ddd', 'eee', 'fff']

    [['abc', 'abc', 'abc'],
     ['def', 'def', 'def']]
    """
    m = len(array)
    n = len(array[0])
    
    for j in xrange(start, n, size):
        
        if size == 1:
            column = ''
        else:
            column = []
            
        for i in xrange(m):
            item = array[i][j: j + size]
            
            if size == 1:
                column += item
            else:
                column.append(item)
        
        yield column


#re-implement without numpy?
        
#def member_iter(ranges, start = 1, end = 1000000):
    
#    rng = numpy.array(ranges)
    
#    for i in range(start, end + 1):
        #if ranges == []:
        #    yield False
        #else:
#        equal = i == rng
        
#        if sum(equal) == 1:
#            yield True
            
#        else:
#            state = i >= rng
            
#            if sum(state) % 2 == 0:
#                yield False
#            else:
#                yield True
                

def non_overlapping_iter(seq, size, start = 0):

    lenseq = len(seq)

    for i in xrange(start, lenseq):
        rem = (i - start) % size
        
        if i == start:
            winseq = seq[i]
        elif rem == 0:
            yield winseq
            winseq = seq[i]
        else:
            winseq += seq[i]

    yield winseq
            

