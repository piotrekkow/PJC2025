classDiagram
    Simulation *-- Network

    Edge --> "0..1" Waypoint : in
    Edge --> "0..1" Waypoint : out
    Edge --> "0..n (2..n if out=0..1)" Junction : in
    Edge --> "0..n (2..n if in=0..1)" Junction : out
    Edge --o Segment
    
    Network *-- "0..n" Node
    Network *-- "0..n" Intersection

    Node *-- "1..n" Vertex    

    Node --> "1" Segment : in
    Node o-- "1" Segment : out
    
    Node --> "0..n" Intersection : intermediate
    Node --> "1..n (2..n if outlet=1)" Intersection : inlet
    Node --> "1..n (2..n if inlet=1)" Intersection : outlet

    
    Segment --> Intersection

    Vertex <|-- Waypoint
    Vertex <|-- Junction