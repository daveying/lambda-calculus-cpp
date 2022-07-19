#ifndef META_HPP__
#define META_HPP__

#include <utility>

namespace meta
{

// λ calculus in C++ mete programming

// Identity (λf.f)
struct I
{
    template <typename T>
    using call = T;
};

// MockingBird (λf.ff)
// self-application
// And
struct M
{
    template <typename T>
    using call = typename T::template call<T>;
};

using IM = I::call<M>;

// I(M) == M
static_assert(std::is_same<IM, M>::value, "");
// M(I) == I
static_assert(std::is_same<M::call<I>, I>::value, "");

// Kestrel (λab.a)
// first, const
// True
struct K
{
    template <typename T>
    struct _lambda_
    {
        template <typename T2>
        using call = T;
    };

    template <typename T>
    using call = _lambda_<T>;
};

// K(I)(M) == I
static_assert(std::is_same<K::call<I>::call<M>, I>::value, "");
// K(M)(I) == M
static_assert(std::is_same<K::call<M>::call<I>, M>::value, "");
// K(K)(M) == K
static_assert(std::is_same<K::call<K>::call<M>, K>::value, "");

using Const = K;
using First = K;
using True = K;

// K5 = Const(5)
using K5 = Const::call<std::integral_constant<int, 5>>;
// K5(M)
static_assert(K5::call<M>::value == 5, "");
// K5(7)
static_assert(K5::call<std::integral_constant<int, 7>>::value == 5, "");

// Kite (λab.b)
// second
// False
struct KI
{
    template <typename T>
    struct _lambda_
    {
        template <typename T2>
        using call = T2;
    };

    template <typename T>
    using call = _lambda_<T>;
};

using Second = KI;
using False = KI;
using _KI_ = K::call<I>;

// KI(I)(M) == M
// K(I)(I)(M) == M
static_assert(std::is_same<KI::call<I>::call<M>, M>::value, "");
// KI(M)(I) == I
// K(I)(M)(I) == I
static_assert(std::is_same<KI::call<M>::call<I>, I>::value, "");
// KI(M)(KI) == KI
// K(I)(M)(KI) == KI
static_assert(std::is_same<KI::call<M>::call<KI>, KI>::value, "");

// K(I)(I)(M) == _KI_(I)(M)
static_assert(std::is_same<KI::call<I>::call<M>, _KI_::call<I>::call<M>>::value, "");
// K(I)(M)(I) == _KI_(M)(I)
static_assert(std::is_same<KI::call<M>::call<I>, _KI_::call<M>::call<I>>::value, "");
// K(I)(M)(KI) == _KI_(M)(KI)
static_assert(std::is_same<KI::call<M>::call<KI>, _KI_::call<M>::call<KI>>::value, "");


// Cardinal (λfab.fba)
// Not
struct C
{
    template <typename F>
    struct _lambda_
    {
        template <typename A>
        struct _lambda1_
        {
            template <typename B>
            using call = typename F::template call<B>::template call<A>;
        };

        template <typename A>
        using call = _lambda1_<A>;
    };

    template <typename F>
    using call = _lambda_<F>;
};

// C(K)(I)(M) == K(M)(I) == M
static_assert(std::is_same<C::call<K>::call<I>::call<M>, M>::value, "");

using Flip = C;

using CK = C::call<K>;

// KI == CK
// K(I)(I)(M) == CK(I)(M)
static_assert(std::is_same<KI::call<I>::call<M>, CK::call<I>::call<M>>::value, "");
// K(I)(M)(I) == CK(M)(I)
static_assert(std::is_same<KI::call<M>::call<I>, CK::call<M>::call<I>>::value, "");
// K(I)(M)(KI) == CK(M)(KI)
static_assert(std::is_same<KI::call<M>::call<KI>, CK::call<M>::call<KI>>::value, "");

// Not (λp.pFT)
// Cardianl
struct Not
{
    // T should be either K or KI
    template <typename T>
    using call = typename T::template call<False>::template call<True>;
};

// intential equality v.s. extensional equality
// Not(True) == False   |=> Not(True) is intential equal to False
static_assert(std::is_same<Not::call<True>, False>::value, "");
// Not(False) == True
static_assert(std::is_same<Not::call<False>, True>::value, "");

// C(True)(I)(M) == False(I)(M) == M  |=> C(True) is extensional equal to False
// C::call<True> generates new "function" that is different than False
// but they are extensional equal, and given the same input, they generate the same output
static_assert(std::is_same<C::call<True>::call<I>::call<M>, M>::value, "");
// C(False)(I)(M) == True(I)(M) == I
static_assert(std::is_same<C::call<False>::call<I>::call<M>, I>::value, "");

// And (λpq.pqp)
// P and Q should be either K or KI
struct And
{
    template <typename P>
    struct _lambda_
    {
        template <typename Q>
        using call = typename P::template call<Q>::template call<P>;
    };

    template <typename P>
    using call = _lambda_<P>;
};

// And(True)(True) == True
static_assert(std::is_same<And::call<True>::call<True>, True>::value, "");
// And(True)(False) == False
static_assert(std::is_same<And::call<True>::call<False>, False>::value, "");
// And(False)(True) == False
static_assert(std::is_same<And::call<False>::call<True>, False>::value, "");
// And(False)(False) == False
static_assert(std::is_same<And::call<False>::call<False>, False>::value, "");

// Or (λpq.ppq)
// M*
// P and Q should be either K or KI
struct Or
{
    template <typename P>
    struct _lambda_
    {
        template <typename Q>
        using call = typename P::template call<P>::template call<Q>;
    };

    template <typename P>
    using call = _lambda_<P>;
};

// Or(True)(True) == True
static_assert(std::is_same<Or::call<True>::call<True>, True>::value, "");
// Or(True)(False) == True
static_assert(std::is_same<Or::call<True>::call<False>, True>::value, "");
// Or(False)(True) == True
static_assert(std::is_same<Or::call<False>::call<True>, True>::value, "");
// Or(False)(False) == False
static_assert(std::is_same<Or::call<False>::call<False>, False>::value, "");

// M(True)(True) == True
static_assert(std::is_same<M::call<True>::call<True>, True>::value, "");
// M(True)(False) == True
static_assert(std::is_same<M::call<True>::call<False>, True>::value, "");
// M(False)(True) == True
static_assert(std::is_same<M::call<False>::call<True>, True>::value, "");
// M(False)(False) == False
static_assert(std::is_same<M::call<False>::call<False>, False>::value, "");

// NXor, λpq.p(qTF)(qFT)  <=> λpq.pq(NOT(q))
// Boolean equality
// P and Q should be either K or KI
struct Beq
{
    template <typename P>
    struct _lambda_
    {
        template <typename Q>
        using call = typename P::template call<Q>::template call<Not::call<Q>>;
    };

    template <typename P>
    using call = _lambda_<P>;
};

using NXor = Beq;

// Beq(True)(True) == True
// NXor(True)(True) == True
static_assert(std::is_same<Beq::call<True>::call<True>, True>::value, "");
// Beq(True)(False) == False
// NXor(True)(False) == False
static_assert(std::is_same<Beq::call<True>::call<False>, False>::value, "");
// Beq(False)(True) == False
// NXor(False)(True) == False
static_assert(std::is_same<Beq::call<False>::call<True>, False>::value, "");
// Beq(False)(False) == True
// NXor(False)(False) == True
static_assert(std::is_same<Beq::call<False>::call<False>, True>::value, "");

// !(p && q) == (!p) || (!q)
// Beq(Not(And(p)(q))) (Or(Not(p))(Not(q))) == True

using p0 = False;
using p1 = True;
using q0 = False;
using q1 = True;

//                         Beq(      Not(      And(p)(q)              ))  (    Or(Not(p))                   (Not(q))       )
static_assert(std::is_same<Beq::call<Not::call<And::call<p0>::call<q0>>>::call<Or::call<Not::call<p0>>::call<Not::call<q0>>>, True>::value, "");
static_assert(std::is_same<Beq::call<Not::call<And::call<p0>::call<q1>>>::call<Or::call<Not::call<p0>>::call<Not::call<q1>>>, True>::value, "");
static_assert(std::is_same<Beq::call<Not::call<And::call<p1>::call<q0>>>::call<Or::call<Not::call<p1>>::call<Not::call<q0>>>, True>::value, "");
static_assert(std::is_same<Beq::call<Not::call<And::call<p1>::call<q1>>>::call<Or::call<Not::call<p1>>::call<Not::call<q1>>>, True>::value, "");

// Church encodings of numerals

// λfa.a
using Zero = KI;
using N0 = Zero;
// Zero(Not)(True) == False
static_assert(std::is_same<Zero::call<Not>::call<True>, True>::value, "");

// λfa.fa
// Once 1
// I*
struct Once
{
    template <typename F>
    struct _lambda_
    {
        template <typename A>
        using call = typename F::template call<A>;
    };

    template <typename F>
    using call = _lambda_<F>;
};
using N1 = Once;

static_assert(Once::call<I>::call<std::integral_constant<int, 5>>::value == 5, "");
// Once(Not)(True) == Not(True) == False
static_assert(std::is_same<Once::call<Not>::call<True>, False>::value, "");

// λfa.f(fa)
// Twice 2
struct Twice
{
    template <typename F>
    struct _lambda_
    {
        template <typename A>
        using call = typename F::template call<typename F::template call<A>>;
    };

    template <typename F>
    using call = _lambda_<F>;
};
using N2 = Twice;

// Twice(Not)(True) == Not(Not(True)) == True
static_assert(std::is_same<Twice::call<Not>::call<True>, True>::value, "");

// λfa.f(f(fa))
// Thrice 2
struct Thrice
{
    template <typename F>
    struct _lambda_
    {
        template <typename A>
        using call = typename F::template call<typename F::template call<typename F::template call<A>>>;
    };

    template <typename F>
    using call = _lambda_<F>;
};
using N3 = Thrice;

// Thrice(Not)(True) == Not(Not(Not(True))) == False
static_assert(std::is_same<Thrice::call<Not>::call<True>, False>::value, "");

// λfa.f(f(f(fa)))
// Fourfold 4
struct Fourfold
{
    template <typename F>
    struct _lambda_
    {
        template <typename A>
        using call = typename F::template call<typename F::template call<typename F::template call<typename F::template call<A>>>>;
    };

    template <typename F>
    using call = _lambda_<F>;
};
using N4 = Fourfold;

// Fourfold(Not)(True) == Not(Not(Not(Not(True)))) == True
static_assert(std::is_same<Fourfold::call<Not>::call<True>, True>::value, "");

// λfa.f(f(f(f(fa))))
// Fivefold 4
struct Fivefold
{
    template <typename F>
    struct _lambda_
    {
        template <typename A>
        using call = typename F::template call<typename F::template call<typename F::template call<typename F::template call<typename F::template call<A>>>>>;
    };

    template <typename F>
    using call = _lambda_<F>;
};
using N5 = Fivefold;

// Fivefold(Not)(True) == Not(Not(Not(Not(Not(True))))) == False
static_assert(std::is_same<Fivefold::call<Not>::call<True>, False>::value, "");

// Succ, +1 for Church numerals, Peano numbers
// λnfa.f(nfa)
struct Succ
{
    template <typename N>
    struct _lambda_
    {
        template <typename F>
        struct _lambda1_
        {
            template <typename A>
            using call = typename F::template call<typename N::template call<F>::template call<A>>;
        };

        template <typename F>
        using call = _lambda1_<F>;
    };

    template <typename N>
    using call = _lambda_<N>;
};

// Succ(Zero)(Not)(True) == Once(Not)(True) == Not(True) == False
static_assert(std::is_same<Succ::call<Zero>::call<Not>::call<True>, False>::value, "");
// Succ(Succ)(Zero)(Not)(True) == Twice(Not)(True) == Not(Not(True)) == True
static_assert(std::is_same<Succ::call<Succ>::call<Zero>::call<Not>::call<True>, True>::value, "");
// Succ(Succ)(Succ)(Zero)(Not)(True) == Thrice(Not)(True) == Not(Not(Not(True))) == False
static_assert(std::is_same<Succ::call<Succ>::call<Succ>::call<Zero>::call<Not>::call<True>, False>::value, "");

template <typename MetaFunc>
struct Callable
{
    template <typename T>
    using call = decltype(MetaFunc::template call<T>());
};

struct IncreFunc
{
    template <typename T>
    static constexpr auto call() { return std::integral_constant<int, T::value + 1>{}; }
};

using Inc = Callable<IncreFunc>;

static_assert(Zero::call<Inc>::call<std::integral_constant<int, 0>>::value == 0, "");
static_assert(Once::call<Inc>::call<std::integral_constant<int, 0>>::value == 1, "");
static_assert(Twice::call<Inc>::call<std::integral_constant<int, 0>>::value == 2, "");
static_assert(Thrice::call<Inc>::call<std::integral_constant<int, 0>>::value == 3, "");

struct ToCppNum
{
    template <typename N>
    using call = typename N::template call<Inc>::template call<std::integral_constant<int, 0>>;
};

static_assert(ToCppNum::call<N0>::value == 0, "");
static_assert(ToCppNum::call<N1>::value == 1, "");
static_assert(ToCppNum::call<N2>::value == 2, "");
static_assert(ToCppNum::call<N3>::value == 3, "");
static_assert(ToCppNum::call<N4>::value == 4, "");
static_assert(ToCppNum::call<N5>::value == 5, "");
static_assert(ToCppNum::call<Succ::call<N0>>::value == 1, "");
static_assert(ToCppNum::call<Succ::call<Succ::call<N0>>>::value == 2, "");
static_assert(ToCppNum::call<Succ::call<N5>>::value == 6, "");
static_assert(ToCppNum::call<Succ::call<Succ::call<N5>>>::value == 7, "");

// BlueBird
// λfga.f(ga)
// Mult
// function composition
struct B
{
    template <typename F>
    struct _lambda_
    {
        template <typename G>
        struct _lambda1_
        {
            template <typename A>
            using call = typename F::template call<typename G::template call<A>>;
        };

        template <typename G>
        using call = _lambda1_<G>;
    };

    template <typename F>
    using call = _lambda_<F>;
};

// B(Not)(Not)(True) == Not(Not(True)) == True
static_assert(std::is_same<B::call<Not>::call<Not>::call<True>, True>::value, "");
// B(ToCppNum)(Succ)(N4) == ToCppNum(Succ(N4))
static_assert(std::is_same<B::call<ToCppNum>::call<Succ>::call<N4>, ToCppNum::call<Succ::call<N4>>>::value, "");
static_assert(B::call<ToCppNum>::call<Succ>::call<N4>::value == 5, "");

using N6 = Succ::call<N5>;
using N7 = Succ::call<N6>;

// Succ using BlueBird
// λnf.Bf(nf)
struct _Succ_
{
    template <typename N>
    struct _lambda_
    {
        template <typename F>
        using call = typename B::template call<F>::template call<typename N::template call<F>>;
    };
    template <typename N>
    using call = _lambda_<N>;
};

static_assert(ToCppNum::call<_Succ_::call<N0>>::value == 1, "");
static_assert(ToCppNum::call<_Succ_::call<_Succ_::call<N0>>>::value == 2, "");
static_assert(ToCppNum::call<_Succ_::call<N5>>::value == 6, "");
static_assert(ToCppNum::call<_Succ_::call<_Succ_::call<N5>>>::value == 7, "");

// Add
// λnk.n Succ k
struct Add
{
    template <typename N>
    struct _lambda_
    {
        template <typename K>
        using call = typename N::template call<Succ>::template call<K>;
    };
    template <typename N>
    using call = _lambda_<N>;
};

static_assert(ToCppNum::call<Add::call<N2>::call<N3>>::value == 5, "");
static_assert(ToCppNum::call<Add::call<N6>::call<N7>>::value == 13, "");

// Mult N2 N3 f a = N2 (N3 f) a
// Mult N2 N3 f = N2 (N3 f)
// Mult N2 N3 f = B N2 N3 f
// Mult N2 N3 = B N2 N3
// Mult = B
using Mult = B;

static_assert(ToCppNum::call<Mult::call<N2>::call<N3>>::value == 6, "");
static_assert(ToCppNum::call<Mult::call<N6>::call<N7>>::value == 42, "");

// Pow, λnk.kn
// Thrush
struct Th
{
    template <typename N>
    struct _lambda_
    {
        template <typename K>
        using call = typename K::template call<N>;
    };
    template <typename N>
    using call = _lambda_<N>;
};
using Pow = Th;

static_assert(ToCppNum::call<Pow::call<N2>::call<N3>>::value == 8, "");
static_assert(ToCppNum::call<Pow::call<N2>::call<N4>>::value == 16, "");

using N16 = Pow::call<N2>::call<N4>;

// IsZero
// λn.n(KF)T
struct IsZero
{
    template <typename N>
    using call = typename N::template call<typename K::template call<False>>::template call<True>;
};

static_assert(std::is_same<IsZero::call<N0>, True>::value, "");
static_assert(std::is_same<IsZero::call<N1>, False>::value, "");
static_assert(std::is_same<IsZero::call<N2>, False>::value, "");
static_assert(std::is_same<IsZero::call<N3>, False>::value, "");
static_assert(std::is_same<IsZero::call<N4>, False>::value, "");
static_assert(std::is_same<IsZero::call<N5>, False>::value, "");
static_assert(std::is_same<IsZero::call<N6>, False>::value, "");
static_assert(std::is_same<IsZero::call<N7>, False>::value, "");
static_assert(std::is_same<IsZero::call<N16>, False>::value, "");

// Vireo
// λabf.fab
struct V
{
    template <typename A>
    struct _lambda_
    {
        template <typename B>
        struct _lambda1_
        {
            template <typename F>
            using call = typename F::template call<A>::template call<B>;
        };

        template <typename B>
        using call = _lambda1_<B>;
    };

    template <typename A>
    using call = _lambda_<A>;
};

using Pair = V;

using VIM = V::call<I>::call<M>;

// V(I)(M)(K) = K(I)(M) = I
static_assert(std::is_same<VIM::call<K>, I>::value, "");
// V(I)(M)(KI) = KI(I)(M) = M
static_assert(std::is_same<VIM::call<KI>, M>::value, "");

// Fst λp.pK
// p is Vireo
struct Fst
{
    template <typename P>
    using call = typename P::template call<K>;
};

// Snd λp.pKI
// p is Vireo
struct Snd
{
    template <typename P>
    using call = typename P::template call<KI>;
};

// Fst(V(I)(M)) = I
static_assert(std::is_same<Fst::call<VIM>, I>::value, "");
// Snd(V(I)(M)) = KI(I)(M) = M
static_assert(std::is_same<Snd::call<VIM>, M>::value, "");

using InjectPair = Pair::call<std::tuple<void>>::call<std::tuple<int, float>>;

static_assert(std::is_same<Fst::call<InjectPair>, std::tuple<void>>::value, "");
static_assert(std::is_same<Snd::call<InjectPair>, std::tuple<int, float>>::value, "");

} // namespace meta

#endif // META_HPP__
