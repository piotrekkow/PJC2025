classDiagram
    %% Core infrastructure
    class SimulationManager {
        +update(deltaTime)
        +initialize()
        +run()
    }
    
    %% Rendering Domain
    class Renderer {
        -camera: Camera2D
        -debugMode: bool
        +initialize()
        +beginFrame()
        +endFrame()
        +renderNetwork(network)
        +renderVehicles(vehicles)
        +renderTrafficControls(controls)
        +toggleDebugMode()
        +handleCameraInput(deltaTime)
        +zoomIn(amount)
        +zoomOut(amount)
        +panCamera(direction)
    }
    
    %% Network Infrastructure Domain
    class NetworkManager {
        -vertices: Vector~unique_ptr~Vertex~~
        -edges: Vector~unique_ptr~Edge~~
        -nodes: Vector~unique_ptr~Node~~
        +addVertex(position): Vertex*
        +addEdge(source, dest): Edge*
        +addNode(position, laneCount, tangent): Node*
        +draw(debug)
    }
    
    class Vertex {
        -position: Vector2
        -inEdges: Vector~Edge*~
        -outEdges: Vector~Edge*~
        +pos(): Vector2&
        +addIn(edge)
        +addOut(edge)
    }
    
    class Edge {
        -source: Vertex*
        -destination: Vertex*
        -length: float
        -properties: EdgeProperties
        +getLength(): float
        +getDest(): Vertex*
    }
    
    class Node {
        -position: Vector2
        -tangent: Vector2
        -vertices: Vector~Vertex*~
        -laneCount: int
        +pos(): Vector2
        +normal(): Vector2
        +drawAxes()
    }
    
    class EdgeProperties {
        -speedLimit: float
        -laneWidth: float
        -priority: int
    }
    
    %% Vehicle Dynamics Domain
    class VehicleManager {
        -vehicles: Vector~unique_ptr~Vehicle~~
        +update(deltaTime)
        +createVehicle(path, speed): Vehicle*
        +removeVehicle(vehicle)
        +draw()
    }
    
    class Vehicle {
        -path: Path
        -currentEdge: Edge*
        -distanceAlongEdge: float
        -speed: float
        -maxSpeed: float
        -acceleration: float
        -deceleration: float
        -state: VehicleState
        -size: Vector2
        +update(deltaTime)
        +draw()
        +getPosition(): Vector2
        -followPath(deltaTime)
        -checkForYielding(): bool
    }
    
    class VehicleState {
        <<enumeration>>
        MOVING
        YIELDING
        STOPPED
    }
    
    %% Traffic Flow Domain
    class TrafficFlowManager {
        -vehicleGenerators: Vector~VehicleGenerator~
        -edgeVehicleMap: Map~Edge*, Vector~Vehicle*~~
        +update(deltaTime)
        +registerVehicle(vehicle, edge)
        +unregisterVehicle(vehicle, edge)
        +updateVehicleEdge(vehicle, oldEdge, newEdge)
        +getVehiclesOnEdge(edge): Vector~Vehicle*~
        +checkCollisions()
    }
    
    class VehicleGenerator {
        -source: Vertex*
        -vehiclesPerHour: float
        -timeToNextVehicle: float
        +update(deltaTime): Vehicle*
        +setFlow(vehiclesPerHour)
    }
    
    %% Yield/Priority Domain
    class IntersectionManager {
        -intersections: Map~Vertex*, IntersectionRules~
        +resolveConflicts(vertex): PriorityList
        +addRules(vertex, rules)
        +shouldYield(vehicle1, vehicle2, vertex): bool
    }
    
    class IntersectionRules {
        -priorityEdges: Vector~Edge*~
        -yieldEdges: Vector~Edge*~
        -rightOfWay: bool
        +evaluatePriority(edge1, edge2): int
    }
    
    %% Pathfinding Domain
    class PathfindingService {
        -network: NetworkManager*
        -routeCache: Map~pair~Vertex*,Vertex*~, Path~
        +findPath(source, dest): Path
        +findShortestPath(source, dest): Path
        +findFastestPath(source, dest): Path
    }
    
    class Path {
        -edges: Vector~Edge*~
        -source: Vertex*
        -destination: Vertex*
        -length: float
        +getEdges(): const Vector~Edge*~&
        +getLength(): float
    }
    
    %% Traffic Control Domain
    class TrafficControlManager {
        -trafficLights: Vector~TrafficLight~
        -signs: Vector~TrafficSign~
        +update(deltaTime)
        +createTrafficLight(position, phases): TrafficLight*
        +createSign(position, type): TrafficSign*
    }
    
    class TrafficLight {
        -position: Vector2
        -phases: Vector~LightPhase~
        -currentPhase: int
        -timer: float
        +update(deltaTime)
        +getCurrentState(): LightState
        +getPosition(): Vector2
    }
    
    %% Simulation Control Domain
    class SimulationClock {
        -time: float
        -timeScale: float
        -paused: bool
        +update(realTimeDelta)
        +getTime(): float
        +setTimeScale(scale)
        +pause()
        +resume()
    }
    
    
    
    %% Relationships
    SimulationManager --> NetworkManager
    SimulationManager --> VehicleManager
    SimulationManager --> TrafficFlowManager
    SimulationManager --> IntersectionManager
    SimulationManager --> PathfindingService
    SimulationManager --> TrafficControlManager
    SimulationManager --> SimulationClock
    SimulationManager --> Renderer
    
    Renderer ..> NetworkManager
    Renderer ..> VehicleManager
    Renderer ..> TrafficControlManager
    
    NetworkManager --> "many" Vertex
    NetworkManager --> "many" Edge
    NetworkManager --> "many" Node
    Edge --> "1" EdgeProperties
    
    Node --> "many" Vertex
    Edge --> "1" Vertex : source
    Edge --> "1" Vertex : destination
    
    VehicleManager --> "many" Vehicle
    Vehicle --> "1" Path
    Vehicle --> "1" VehicleState
    Vehicle --> "1" Edge : currentEdge
    
    TrafficFlowManager --> "many" VehicleGenerator
    VehicleGenerator --> "1" Vertex : source
    VehicleGenerator ..> Vehicle : creates
    
    IntersectionManager --> "many" IntersectionRules
    IntersectionRules --> "many" Edge
    
    PathfindingService --> "1" NetworkManager
    PathfindingService ..> Path : creates
    Path --> "many" Edge
    Path --> "1" Vertex : source
    Path --> "1" Vertex : destination
    
    TrafficControlManager --> "many" TrafficLight
    TrafficLight --> "many" LightPhase