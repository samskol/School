# wiki

def viterbi(obs, states, start_p, trans_p, emit_p):
    V = [{}]
    path = {}
    
    # Initialize base cases (t == 0)
    for y in states:
        V[0][y] = start_p[y] * emit_p[y][obs[0]]
        path[y] = [y]
    
    # Run Viterbi for t > 0
    for t in range(1, len(obs)):
        V.append({})
        newpath = {}

        for y in states:
            (prob, state) = max((V[t-1][y0] * trans_p[y0][y] * emit_p[y][obs[t]], y0) for y0 in states)
            V[t][y] = prob
            newpath[y] = path[state] + [y]

        # Don't need to remember the old paths
        path = newpath
    
    # Return the most likely sequence over the given time frame
    n = len(obs) - 1
    print(*dptable(V), sep='')
    (prob, state) = max((V[n][y], y) for y in states)
    return (prob, path[state])

# Don't study this; it just prints a table of the steps.
def dptable(V):
    yield "    "
    yield " ".join(("%7d" % i) for i in range(len(V)))
    yield "\n"
    for y in V[0]:
        yield "%.5s: " % y
        yield " ".join("%.7s" % ("%f" % v[y]) for v in V)
        yield "\n"
