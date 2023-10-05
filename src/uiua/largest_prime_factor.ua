# Slow solution
# is ← +×.∶×,⇡⌈÷,+→∶×¯1×..
# sz ← ×¬∊⇡⧻,
# s ← ⍥(↘1⍥(→sz ∶is ⊡0 ∶⧻∶,,)∶→⊡∶⊢.∶,)⧻.↘2⇡⌊√⧻.▽∶ 1
# s 1000
# ▽∶+2⇡⧻.↘2;

# Faster solution because of better removeComposite
N ← 600851475143
N ← 100
findComposite ← +×.∶×,⇡⌈÷,+→∶×¯1×..
removeComposite ← ⍜(⊏)'×0
primes ← ⍥(↘1⍥(→removeComposite ∶findComposite ⊡0 ∶⧻∶,,)∶→⊡∶⊢.∶,)⧻.↘2⇡⌊√⧻.▽∶ 1
primes ⌊√ N
&s "done"
# ▽∶+2⇡⧻.↘2;
# &s "putting data"
# ▽ =0◿ N .
