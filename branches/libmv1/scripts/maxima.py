"""
Convert maxima matrix literals into Python. This script is a hack and only
exists for people curious about how I created the 100k monstrosity that is the
metric upgrade code.
Keir Mierle 2007
"""
def maxima_matrix_to_python_literal(s, name='A'):
    s = s.strip()
    assert(s.startswith('matrix(['))
    assert(s.endswith(']);'))
    inner = s[8:-3]
    rows = inner.split('],[')
    nrows = len(rows)
    mat = [row.split(',') for row in rows]
    ncols = len(mat[0])
    print 'nrows',len(rows)
    for row in mat:
        assert len(row) == ncols
    for i in range(nrows):
        for j in range(ncols):
            print '%s[%s,%s] = %s' % (name, i,j,mat[i][j].replace('^','**'))

def maxima_matrix_to_c_literal(s, name='A'):
    # note that you have to manually reshape, this only makes a flat version
    s = s.strip()
    assert(s.startswith('matrix(['))
    assert(s.endswith(']);'))
    inner = s[8:-3]
    inner = inner.replace('],[',',')
    import re
    inner = re.sub(r'([wp]\d\d?)\^(\d)',r'pow(\1,\2)',inner)
    print 'double %s[] = {%s};' % (name,inner)

if __name__ == '__main__':
    print '// DO NOT EDIT THIS FILE: AUTOMATICALLY GENERATED BY maxima.py'
    # Note that A_matrix and B_matrix are 
    maxima_matrix_to_c_literal(file('A_matrix').read(),'A')
    maxima_matrix_to_c_literal(file('B_matrix').read(),'B')
