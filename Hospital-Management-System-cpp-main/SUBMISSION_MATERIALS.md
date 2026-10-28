# Hospital Management System — Submission Materials

## Project report

### 1. Objective

This C++17 console project models selected hospital workflows for a college OOP and DSA demonstration. It lets an administrator manage staff, lets patients create priority appointments and feedback, supports admissions and nurse notes, tracks emergency cases, and provides navigation and prefix search. It uses local CSV files for selected data so records can survive a normal application restart.

This is a teaching/demo system. It must not be used with real patient data.

### 2. How to build and run

From the project root:

```bash
cmake -S . -B build
cmake --build build
./build/HospitalApp
```

The initial demonstration password is `admin123` unless a local password file has changed it. The password is stored as plain text for this classroom project.

### 3. Architecture

`HospitalApp` is the controller for authentication and the central dashboard. It owns and routes work to these modules:

| Module | Responsibility | Main data structure |
| --- | --- | --- |
| `HospitalDirectory` | Staff records | AVL tree indexed by staff ID |
| `PriorityQueue` | Appointments, admissions, old patients, feedback | Max-heap, hash map, stack, queue |
| `EmergencyManagement` | Active and discharged emergency cases | Stacks |
| `HospitalGraph` | Routes between hospital locations | Adjacency list with Dijkstra's algorithm |
| `Trie` | Prefix search | Trie |
| `PatientHistory` | Previous/next completed check-ups in one session | Doubly linked list |
| `ShiftRotator` | On-call rotation | Circular linked list |
| `Logger` | Local event log | Singleton with mutex |

### 4. OOP concepts demonstrated

- **Encapsulation:** Each component keeps its internal data structure private and exposes focused public operations.
- **Composition:** `HospitalApp` composes the independent service classes and coordinates the menus.
- **Abstraction:** Each class exposes an operational interface such as `bookAppointment`, `findShortestPath`, or `nextShift` without exposing implementation details.
- **Resource management:** `shared_ptr` manages dynamic nodes and records; destructors save selected module state at normal shutdown.
- **Single responsibility:** Data-structure logic, billing, logging, UI helpers, and app routing live in separate files.

### 5. DSA used and why

| Data structure / algorithm | Use in this project | Demonstrated complexity |
| --- | --- | --- |
| AVL tree | Staff records ordered by ID | Search, insert, delete: O(log n) |
| Binary max-heap | High-priority appointments first | Insert/remove: O(log n), next patient: O(1) |
| Hash map | Find admitted patient by ID | Average lookup: O(1) |
| Stack | Emergency workflow and old-patient history | Push/pop: O(1) |
| Queue | Complaint order | Enqueue/dequeue: O(1) |
| Trie | Prefix search suggestions | O(L) to traverse a prefix of length L, plus result collection |
| Graph + Dijkstra | Shortest route between predefined locations | O((V + E) log V) with the priority queue |
| Doubly linked list | Previous/next viewing history | Move backward/forward: O(1) |
| Circular linked list | Repeating on-call rotation | Advance to next doctor: O(1) |

### 6. Persistence and validation

- Staff, appointments, admissions, complaints, and emergency data are written to CSV files under `data/` during normal shutdown and loaded at startup.
- Password changes are saved to `data/password.txt`.
- The application validates numeric input, rejects required text fields that are blank, rejects non-positive IDs, and prevents duplicate active appointment/staff/ER IDs where applicable.
- Emergency persistence saves both active and discharged stacks. The loader accepts the earlier discharged-record CSV format as well as the current one.
- Commas in saved free text are replaced with spaces. Therefore the CSV format does not support commas inside a field.

### 7. Verified tests

All tests below used fictional records in isolated temporary directories.

| Test | Result observed |
| --- | --- |
| Build with CMake and Apple Clang | Passed |
| Invalid numeric input | Rejected with a validation message |
| Negative appointment ID | Rejected |
| Appointment cancellation | Passed; matching active appointment was removed |
| Appointment creation, check-up, and history | Passed; completed check-up appeared in in-session history |
| Appointment admission and nurse-note update | Passed |
| Staff add, display, and deletion | Passed |
| ER registration and discharge | Passed |
| Active ER persistence across restart | Passed |
| ER stack order across restart | Passed; most recently registered case remained first |
| Navigation Entrance to ICU | Passed; displayed 60 m route through Reception and Emergency |
| Search prefix `car` | Passed; returned Cardiology |
| Password change across restart | Passed; old key was denied and new key granted access |
| Invoice formatting | Passed; displayed currency with two decimal places |
| Blank feedback | Rejected |
| Unknown submenu choice | Rejected with a clear error message |

### 8. Limitations and future work

- Passwords are plain text and are suitable only for a classroom demo. Production software needs salted password hashing, access roles, and audit controls.
- CSV storage is simple and has no transaction handling, encryption, or concurrent multi-user protection.
- The viewing history is in-memory and begins after completed check-ups in the current session.
- Trie search starts with fixed department and doctor names. It does not yet refresh automatically when staff records change.
- The program does not model real clinical decisions, billing rules, patient privacy requirements, or hospital integrations.
- Future work: role-based login, database persistence, dynamic search indexing, automated unit tests, and a richer staff/patient interface.

## Three-member contribution breakdown

Use the following allocation only after replacing the placeholders with your team members' names and confirming that each person can explain their assigned code. The original repository remains attributed to its original author; inherited files should not be represented as newly written by the team.

| Member | Suggested ownership | Evidence to show in demo |
| --- | --- | --- |
| Member 1 — `[Name]` | Application integration, login/password flow, UI menus, CMake/build instructions, input validation | Login, invalid input handling, dashboard navigation, build command |
| Member 2 — `[Name]` | Staff directory and patient-care workflow: AVL records, appointment heap, admissions, feedback, billing output | Add/list/delete staff; schedule/cancel/check up; show nurse update |
| Member 3 — `[Name]` | Emergency stack persistence, graph navigation, trie search, history, shift rotation, test evidence | Register/discharge/restart ER case; route search; prefix search; explain history/rotation |

### Honest attribution note

The repository originated from `Kianis534/Hospital-Management-System-cpp`. The team should describe this submission as an adapted and verified course project, then identify the exact fixes, documentation, testing, and explanation work completed by the team.

## Four-slide presentation plan

### Slide 1 — Hospital Management System

- C++17 console application for OOP and DSA learning
- Team members: `[Name 1]`, `[Name 2]`, `[Name 3]`
- Scope: staff, appointments, admissions, emergency, navigation, search

### Slide 2 — Architecture and DSA

- `HospitalApp` coordinates focused modules
- AVL tree for staff and max-heap for priority appointments
- Hash map for admissions; stacks for emergency; queue for feedback
- Trie for search; graph + Dijkstra for navigation; linked lists for history and shifts

### Slide 3 — Demonstrated workflow and verification

- Login validates access before dashboard entry
- Staff and appointment IDs reject invalid or duplicate active records
- Emergency cases survive restart while preserving stack order
- Tested staff workflow, admission update, ER discharge, navigation, search, password restart, and validation

### Slide 4 — Team work, limitations, and next steps

- Present each member's verified module ownership
- Demo-level persistence uses CSV files
- Plain-text password and in-memory history are known limitations
- Next steps: database, role-based login, automatic search updates, unit tests

## Hinglish presentation script

### Slide 1

“Good morning. Hamara project Hospital Management System hai, jo C++17 mein bana hai. Iska goal hospital ke common workflows ko DSA aur OOP concepts ke through demonstrate karna hai. Isme staff records, appointments, admissions, emergency cases, navigation aur search features hain.”

### Slide 2

“Architecture mein `HospitalApp` central controller hai. Staff directory ke liye AVL tree use kiya hai, jisse records balanced rehte hain. Priority appointments ke liye max-heap use kiya hai. Admissions ke liye hash map, emergency ke liye stack, complaints ke liye queue, aur search ke liye trie use kiya hai. Hospital route ke liye graph aur Dijkstra algorithm use hota hai.”

### Slide 3

“Demo mein pehle login hota hai. Hum invalid input aur duplicate active IDs reject karte hain. Appointment ko check-up ke baad admission ya old-patient history mein move kiya ja sakta hai. Emergency case ko register karke restart ke baad bhi dekh sakte hain. Humne staff management, nurse update, discharge, navigation, search aur password persistence test kiya hai.”

### Slide 4

“Team mein har member ek clear module explain karega. Project mein CSV persistence use hui hai, lekin ye production hospital system nahi hai. Password plain text mein hai aur history session ke andar rehti hai. Future mein database, role-based login, dynamic search aur automated tests add karenge. Thank you.”

## Likely viva questions and accurate answers

1. **Why did you use an AVL tree for staff records?**
   AVL balancing keeps the tree height logarithmic, so staff search, insertion, and deletion remain efficient even when IDs arrive in sorted order.

2. **Why is the appointment queue a max-heap?**
   The highest-priority appointment stays at the root, so the next critical patient can be selected in O(1), while insertion and removal take O(log n).

3. **What is the difference between the appointment heap and admitted-patient hash map?**
   The heap decides the next appointment by priority. The hash map retrieves an admitted patient by ID quickly. They solve different access patterns.

4. **Why use a stack for emergency records?**
   A stack models last-in, first-out handling and also demonstrates push/pop operations. The project keeps active and discharged emergency records in separate stacks.

5. **How does Dijkstra's algorithm work here?**
   Locations are graph vertices and corridor distances are weighted edges. Dijkstra repeatedly expands the currently shortest known route to calculate a shortest path.

6. **What does the trie improve?**
   It shares common prefixes, so the program reaches the prefix node in time proportional to the query length, then collects matching words.

7. **How does persistence work?**
   Selected module state is serialized into CSV files on normal shutdown and loaded in constructors at next startup. The project also validates loaded IDs and required names.

8. **Does this project provide secure authentication?**
   No. It uses a plain-text local password for demonstration only. A real system needs password hashing, authorization roles, encryption, and audit trails.

9. **What happens if a user enters letters where an integer is required?**
   `Utility::getSafeInt` clears the failed input state and prompts again, which prevents the input stream from remaining invalid.

10. **What are the project’s main limitations?**
    It uses CSV rather than a transactional database, has fixed initial search terms, keeps viewing history only for the active session, and does not meet clinical or privacy requirements.
