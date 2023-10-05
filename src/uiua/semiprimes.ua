N ← 100000000
findComposite ← +×.∶×,⇡⌈÷,+→∶×¯1×..
removeComposite ← ⍜(⊏)'×0
primes ← ⍥(↘1⍥(→removeComposite ∶findComposite ⊡0 ∶⧻∶,,)∶→⊡∶⊢.∶,)⧻.↘2⇡⌊√⧻.▽∶ 1
primes ⌊√ N
&s "done"
▽∶+2⇡⧻.↘2;
