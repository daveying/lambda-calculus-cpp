#include <meta.hpp>
#include <gtest/gtest.h>
#include <iostream>

using namespace meta;

struct Nil {};

template <typename G>
struct List
{
    template <typename Interface, typename ImplT>
    auto add(G = G())
    {
        using NewItem = typename Pair::call<Interface>::template call<ImplT>;
        // G has the structure of (Item, (Item, (Item, Nil)))
        using NewG = typename Pair::call<NewItem>::template call<G>;
        return List<NewG>();
    }

    using Graph = G;

    template <typename G2>
    using Head = Fst::call<G2>;

    template <typename G2>
    using Tail = typename std::conditional<std::is_same<G2, Nil>::value, Nil, Snd::call<G2>>::type;

    template <typename G2,
             typename std::enable_if<!std::is_same<G2, Nil>::value, bool>::type = true>
    auto install(List<G2>)
    {
        using NewList = decltype(List<G>().install(List<Tail<G2>>()));
        using NewG2 = typename Pair::call<Head<G2>>::template call<typename NewList::Graph>;
        return List<NewG2>();
    }

    template <typename G2,
             typename std::enable_if<std::is_same<G2, Nil>::value, bool>::type = true>
    auto install(List<G2>)
    {
        return List<G>();
    }
};

using num0 = std::integral_constant<int, 0>;
using num1 = std::integral_constant<int, 1>;
using num2 = std::integral_constant<int, 2>;
using num3 = std::integral_constant<int, 3>;
using num4 = std::integral_constant<int, 4>;
using num5 = std::integral_constant<int, 5>;
using num6 = std::integral_constant<int, 6>;
using num7 = std::integral_constant<int, 7>;

template <typename T>
void printT(T)
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
}

TEST(Meta, List)
{
    // ([2,3], ([0,1], Nil))
    auto graph = List<Nil>().add<num0, num1>().add<num2, num3>();
    // ([6,7], ([4,5], Nil))
    auto graph2 = List<Nil>().add<num4, num5>().add<num6, num7>();
    // ([6,7], ([4,5], ([2,3], ([0,1], Nil))))
    auto combinedGraph = graph.install(graph2);

    printT(graph);
    printT(graph2);
    printT(combinedGraph);

    using CG = decltype(combinedGraph)::Graph;

    // Read the list
    static_assert(std::is_same<
            Fst::call<CG>,
            Pair::call<num6>::call<num7>
        >::value, "");
    static_assert(std::is_same<
            Fst::call<Snd::call<CG>>,
            Pair::call<num4>::call<num5>
        >::value, "");
    static_assert(std::is_same<
            Fst::call<N2::call<Snd>::call<CG>>,
            Pair::call<num2>::call<num3>
        >::value, "");
    static_assert(std::is_same<
            Fst::call<N3::call<Snd>::call<CG>>,
            Pair::call<num0>::call<num1>
        >::value, "");
    static_assert(std::is_same<
            N4::call<Snd>::call<CG>,
            Nil
        >::value, "");
}

TEST(Meta, EmptyListInstall)
{
    auto graph = List<Nil>();
    // ([6,7], ([4,5], Nil))
    auto graph2 = List<Nil>().add<num4, num5>().add<num6, num7>();
    // ([6,7], ([4,5], Nil))
    auto combinedGraph = graph.install(graph2);

    printT(graph);
    printT(graph2);
    printT(combinedGraph);

    using CG = decltype(combinedGraph)::Graph;

    // Read the list
    static_assert(std::is_same<
            Fst::call<CG>,
            Pair::call<num6>::call<num7>
        >::value, "");
    static_assert(std::is_same<
            Fst::call<Snd::call<CG>>,
            Pair::call<num4>::call<num5>
        >::value, "");
    static_assert(std::is_same<
            Snd::call<Snd::call<CG>>,
            Nil
        >::value, "");
}
