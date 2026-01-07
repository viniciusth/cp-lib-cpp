# CP Library Architecture

## Segment Tree Abstraction

The Segment Tree library uses a policy-based design pattern to separate the **Tree Structure** (logic) from the **Algebraic Operations** (data).

```mermaid
classDiagram
    %% Core Abstractions
    class SegTree_Config_ {
        +prod(l, r) S
        +set(p, val) void
        +max_right(l, pred) int
        +min_left(r, pred) int
    }
    class LazySegTree_Config_ {
        +prod(l, r) S
        +apply(l, r, f) void
        +set(p, val) void
    }

    %% The Config Interface
    class Config {
        <<Concept>>
        +S (Data Type)
        +F (Action Type)
        +op(S, S) S
        +e() S
        +mapping(F, S) S
        +composition(F, F) F
        +id() F
    }
    
    %% Relationships
    SegTree_Config_ ..> Config : Depends On
    LazySegTree_Config_ ..> Config : Depends On
    
    %% Helper for Composition
    class Lazy_Action_Data_ {
        <<Synthesizer>>
        Combines separate Action and Data policies
        into a single Config
    }
    
    Lazy_Action_Data_ ..|> Config : Implements
    
    %% Policies
    class DataPolicy {
        <<Monoid>>
        +S Type
        +op(S, S)
        +e()
    }
    
    class ActionPolicy {
        <<Monoid Action>>
        +F Type
        +mapping(F, S)
        +composition(F, F)
        +id()
    }
    
    Lazy_Action_Data_ --> DataPolicy : Uses
    Lazy_Action_Data_ --> ActionPolicy : Uses
    
    %% Concrete Examples
    class RangeSum_T_ {
        +SumNode S
        +op(S, S)
        +e()
    }
    class RangeAdd_T_ {
        +T F
        +mapping(T, S)
        +composition(T, T)
    }
    
    RangeSum_T_ --|> DataPolicy : Example
    RangeAdd_T_ --|> ActionPolicy : Example
```

### Components

1.  **SegTree / LazySegTree**: The container classes. They handle the tree traversal, indexing, and complexity ($O(\log N)$). They don't know *what* they are calculating, only that it follows the Monoid laws.
2.  **Config**: The template argument. It tells the tree:
    *   `S`: What data is stored in nodes.
    *   `op`: How to combine two nodes.
    *   `F`: What lazily applied action looks like.
    *   `mapping`: How an action updates a node.
    *   `composition`: How two actions merge.
3.  **Policies**: Small, reusable structs (e.g., `RangeSum`, `RangeAdd`) that implement parts of the Config.
4.  **Lazy**: A convenience wrapper that stitches a `DataPolicy` and an `ActionPolicy` together to form a full `Config`.
