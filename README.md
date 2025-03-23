# Symulacja Ruchu Drogowego

Projekt implementuje mikro-symulację ruchu drogowego skupiającą się na co najwyżej kilku skrzyżowaniach jednocześnie. Docelowo ma dosyć realistycznie odwzorowywać ruch - inspiracją jest PTV Vissim.

## Założenia Techniczne
- Język: C++
- Grafika: raylib, 2D
- Paradygmat: Programowanie obiektowe
- Dodatki: CMake oraz pliki w .vscode, .devcontainer pozwalają kompilować kod na Github Codespaces (pracę zdalną na tablecie)

## Główne składowe projektu

### 1. Struktura Symulacji
- `Simulation`: Centralny koordynator integrujący wszystkie systemy
  - Zarządza czasem symulacji
  - Obsługuje funkcjonalność pauzy/wznowienia
  - Koordynuje aktualizacje we wszystkich podsystemach
- `Config`: Konfiguracja zawierająca parametry symulacji
  - Wymiary okna, nazwa symulacji, zmienne constexpr używane globalnie (na przykład kolory)
- `Renderer`: System renderowania wykorzystujący raylib

### 2. Zarządzanie ID
- `EntityId`: Klasa łącząca typ encji zdefiniowany w EnitityType i numer id
  - Silne typowanie dla bezpieczeństwa typów w czasie kompilacji
  - Wydajne operacje porównawcze
  - Reprezentacja stringowa do debugowania
- `EntityType`: Klasa enum definiująca wszystkie kategorie encji
  - Elementy infrastruktury (VERTEX, EDGE, NODE, itp.)
  - Agenci (VEHICLE, PEDESTRIAN, itp.)
- `IdGenerator`: Generowanie ID
  - Zarządzanie licznikami dla każdego typu encji
- `BaseEntity`: Abstrakcyjna klasa bazowa dla wszystkich identyfikowalnych obiektów
  - Automatyczne przypisanie ID przez konstruktor
  - Możliwości sprawdzenia typu

### 3. Infrastruktura (Sieć Oparta na Grafie)
- `TrafficNetwork`: Menedżer dla wszystkich komponentów infrastruktury
  - Wyszukiwanie encji przez ID
  - Dodawanie, usuwanie komponentów infrastruktury

- System Wierzchołków:
  - `Vertex`: Reprezentuje punkt w sieci z danymi pozycji
    - `Waypoint`: Maks. 1 krawędź wchodząca i wychodząca
    - `Junction`: Wiele krawędzi wchodzących lub wychodzących
  - `Edge`: Krawędź, łączy wierzchołki i reprezentuje pasy ruchu
    - Połączenie kierunkowe

- System Węzłów (Abstrakcja Wyższego Poziomu):
  - `Node`: Węzęł, grupa wierzchołków w z ustaloną pozycją odniesienia
    - Funkcjonuje jako punkty decyzyjne dla trasowania
    - Może istnieć na wlotach/wylotach skrzyżowań
  - `Segment`: Połączenie między dwoma węzłami (Node)
    - Umożliwia zmiany pasa ruchu między krawędziami

### 4. Agenci
- `AgentManager`: Koordynuje wszystkie agenty w symulacji
  - Wydajne wyszukiwanie pojazdów przez ID
  - Tworzenie pojazdów przez wzorzec fabryki
  - Generowanie ruchu z konfigurowalnymi parametrami
  - Zbieranie statystyk do analizy

- `AgentGenerator`: Generuje agenty z określoną ścieżką w danym wierzchołku

- System Pojazdów:
  - `Agent`: Abstrakcyjna klasa bazowa dla wszystkich typów agentów
    - Pozycja, prędkość i wymiary fizyczne
    - Możliwości podążania ścieżką
    - Integracja z zachowaniem kierowcy
  - Konkretne typy agentów:
    - `Vehicle`: Abstrakcyjna klasa bazowa dla wszystkich typów pojazdów
        - `Car`: Standardowy samochód osobowy
        - `Truck`: Większy pojazd o innych właściwościach fizycznych
  
- System Zachowań:
  - `DriverBehavior`: Enkapsuluje logikę decyzji kierowcy
    - Konfigurowalna agresywność i przestrzeganie zasad
    - Decyzje dotyczące zmiany pasa i przyspieszenia
    - Symulacja czasu reakcji
  - `CollisionDetector`: Logika poruszania się po ścieżce
    - Sprawdza, czy agent może bezpiecznie kontynuować na danej ścieżce, przestrzegając zasad ruchu drogowego
    - System podążania za pojazdem przed
    
- System Ścieżek:
  - `Path`: Reprezentuje zaplanowane trasy dla pojazdów
    - Sekwencja węzłów i segmentów (lub wierzchołków i krawędzi)
    - Śledzenie postępu i obliczanie pozostałej odległości

## Możliwe Rozszerzenia

- **System Skrzyżowań z obsługą sygnalizacji:**
  - `Intersection`: Definiuje skrzyżowanie kontrolowane przez sygnalizację świetlną
    - Zawiera węzły/wierzchołki wejściowe i wyjściowe
    - Integruje się z kontrolerami sygnalizacji świetlnej
  - `TrafficLightController`: Koordynuje światła na skrzyżowaniu
    - Zarządza czasem sygnalizacji i cyklami
  - `TrafficLight`: Kontroluje określone węzły/krawędzie
    - Utrzymuje stan świateł i czas
    - Określa uprawnienia do przejazdu pojazdów

- **Interfejs Użytkownika i Obsługa Myszy/Klawiszy**

- **Adaptacyjne Systemy Sygnalizacji Świetlnej**:
  - Dostosowanie długości sygnału na podstawie informacji z symulowanych detektorów pętli indukcyjnych

- **Dynamiczny Algorytm Znajdowania Ścieżki**:

- **Symulacja Pieszych**:
  - Agent `Pedestrian` z oddzielnymi zasadami zachowania od pojazdów
  - Przejścia dla pieszych i sygnalizacja dla pieszych
  - Interakcja z pojazdami

- **Ruch Rowerowy**:
  - Agent `Bicycle` z unikalnymi charakterystykami ruchu

- **Zbieranie Statystyk**
  - Gromadzenie i analizowanie danych symulacji

- **Wizualizacja Danych**:
  - Heatmapy natężenia ruchu
  - Diagramy przepływu ruchu

## Odnośniki
- PTV Vissim
- Cubic Synchro Studio
- Dz.U. 2019 poz. 2311