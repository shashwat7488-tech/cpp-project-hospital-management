# Project Checkpoint

## Verified starting point

- Branch: `main`
- Latest commit: `a215f09 Initial commit` (`v1.0.0`)
- The checkout has pre-existing, uncommitted implementation and submission-material changes. They were preserved during the final audit.

## Changes completed across the two continuation sessions

- `CMakeLists.txt`: removed the macOS-inappropriate root-level `.exe` copy step. The executable is now used directly from `build/HospitalApp`.
- `include/Utility.h`: added shared trimming and required-text input helpers.
- `include/PriorityQueue.h`, `src/PriorityQueue.cpp`, `src/HospitalApp.cpp`: reject invalid/duplicate active appointment IDs, require essential appointment/check-up details, and add completed check-ups to the in-session viewing history.
- `src/HospitalDirectory.cpp`: validate staff input and loaded staff records, prevent duplicate staff IDs, and ensure the records directory exists before saving.
- `src/EmergencyManagement.cpp`: validate emergency input, prevent duplicate active ER IDs, preserve active ER cases across restarts, and retain compatibility with the earlier discharged-record CSV format.
- `src/EmergencyManagement.cpp`: retain LIFO emergency-stack order after a save/load cycle.
- `src/BillingService.cpp`: format invoice totals with exactly two decimal places.
- `src/HospitalApp.cpp`: show a clear error for invalid submenu choices and reject blank feedback.
- `README.md`: corrected persistence wording and macOS/Linux build/run commands.
- `README.md`: corrected the emergency filenames and plain-text-password wording, added a macOS quick demonstration path, and documented attribution/licensing guidance.
- `SUBMISSION_MATERIALS.md`: added an evidence-based report, DSA/OOP explanation, test checklist, transparent team-role template, presentation outline, Hinglish script, and viva Q&A.
- `submission/Hospital_Management_System_Presentation.pptx`: added and visually reviewed a four-slide editable presentation. Replace `[Name 1]`, `[Name 2]`, and `[Name 3]` before presenting.

## Verification performed

### Final demo-readiness audit (28 October 2026)

- Fresh CMake configuration and build passed: `cmake -S . -B build && cmake --build build --parallel 2`.
- `ctest --test-dir build --output-on-failure` completed successfully but reports no registered CTest tests. Console verification below is therefore the primary evidence.
- The application launched successfully, accepted the configured demo credential, and displayed the Central Dashboard.
- Isolated console runs (with fictional records in temporary folders) passed these main flows:
  - invalid numeric input, failed login, and unknown dashboard choice showed validation/error messages;
  - staff add and staff-directory display;
  - appointment scheduling, priority display, check-up, invoice display, admission, nurse-station display, and nurse-note update;
  - viewing-history display after a completed check-up;
  - emergency registration, restart, triage display, discharge, invoice display, and emergency CSV persistence;
  - navigation from Entrance to ICU returned the expected 60 m route;
  - prefix search for `car` returned Cardiology.
- `SUBMISSION_MATERIALS.md`, `README.md`, and this checkpoint are present and consistent with the implemented scope.
- `submission/Hospital_Management_System_Presentation.pptx` passed ZIP-package integrity checks and contains exactly four slide XML files. Its slide content matches the documented scope.
- No additional source-code fix was required by this final audit.

### Final submission materials (28 October 2026)

- `submission/Hospital_Management_System_Report.docx` was created as an editable seven-page B.Tech project report with a cover page, certificate and acknowledgement templates, contents, architecture, OOP/DSA, persistence, testing, limitations, references, and editable personal placeholders.
- `submission/Hospital_Management_System_Report.pdf` was generated from the DOCX, confirmed readable, and visually reviewed page by page.
- `submission/Hospital_Management_System_Presentation.pptx` was finalized as a four-slide deck. A statement about test-record location was corrected to the factual wording that all test records were fictional; the deck was rendered and visually reviewed after the change.
- `README.md` now links directly to the report, PDF, presentation, and supporting submission material, and clarifies that local data, log, and password files are ignored by Git.
- Local submission commit: `6a2abcd Prepare hospital management system submission`.
- GitHub publication could not be completed: the configured remote is `https://github.com/Kianis534/Hospital-Management-System-cpp.git`, and its write check returned HTTP 403 for the authenticated account. The remote was not changed and no push was attempted after that check.

### Earlier verification retained for context

- `cmake -S . -B build && cmake --build build --parallel 2` — passed with Apple Clang/C++17.
- `git diff --check` — passed.
- Scripted smoke test in a temporary directory — passed:
  - successful login;
  - negative appointment ID rejected;
  - appointment created and checked up;
  - completed check-up appears in viewing history;
  - active emergency case was registered, application restarted, then its triage record was displayed successfully.
- Additional isolated end-to-end checks — passed:
  - staff add/list/delete;
  - appointment cancellation, admission, and nurse-note update;
  - ER discharge;
  - hospital navigation from Entrance to ICU (60 m);
  - prefix search `car` returning Cardiology;
  - invalid numeric input and unknown submenu-choice handling;
  - password change retained after restart;
  - blank feedback rejection;
  - two-decimal invoice output;
  - LIFO emergency order retained after restart.
- Final `cmake --build build --parallel 2` — passed.
- Final isolated launch using `admin123` — passed; the application reached the Central Dashboard and exited normally without altering project data.
- `ctest --test-dir build -N` reports zero registered CTest tests. The verification above is scripted console testing, not a unit-test suite.
- Final presentation verification — passed: the PPTX ZIP package has no errors, has exactly four slide XML files, and all four slides were freshly rendered and visually inspected without clipping or missing content.

## Current uncommitted changes

- `CMakeLists.txt`
- `README.md`
- `include/PriorityQueue.h`
- `include/Utility.h`
- `src/EmergencyManagement.cpp`
- `src/HospitalApp.cpp`
- `src/HospitalDirectory.cpp`
- `src/PriorityQueue.cpp`
- `PROJECT_CHECKPOINT.md`
- `SUBMISSION_MATERIALS.md`
- `submission/Hospital_Management_System_Presentation.pptx`
- `.gitignore`
- `src/BillingService.cpp`

Before the final audit, project data files were not created or altered by the earlier isolated tests. See the audit cleanup note below for the one corrected working-directory mistake during the final audit.

### Audit cleanup note

During the 28 October 2026 audit, one initial scripted run unintentionally used the project root as its working directory before the isolation setup was corrected. It created only fictional, ignored artifacts: `data/admitted.csv`, `data/appointments.csv`, `data/complaints.csv`, `data/emergency_current.csv`, `data/emergency_old.csv`, `data/hospital.log`, and `data/hospital_records.csv`. They must be removed before presenting if a clean data folder is desired. No tracked source file or pre-existing project data was deleted or overwritten. Removal requires the owner's explicit approval because these files now reside in the project data directory.

The project root contains an ignored `HospitalApp.exe` Mach-O executable left from the earlier build configuration. It was not created or modified in these sessions and was deliberately preserved.

## Remaining work / risks

- **Before presenting:** either approve deletion of the seven fictional test artifacts listed above or retain them knowingly. They can cause demonstration records such as `Dr. Audit` to appear in the live application.

- The password remains plain text by design for this demo; it must not be presented as production security.
- History is in-memory only and starts recording after check-ups in the current session.
- Search is seeded with fixed departments/doctors rather than dynamically reflecting staff changes.
- There are no registered automated unit tests. Future work can convert the scripted console checks into unit/integration tests.
- No `LICENSE`, `COPYING`, or `NOTICE` file exists in this checkout. The configured origin is `https://github.com/Kianis534/Hospital-Management-System-cpp.git`; retain attribution and confirm upstream permission before any publication or redistribution.

## Next task

Before submission, replace the three placeholder names in the presentation and contribution table, rehearse each member's assigned explanation, and commit the reviewed changes if desired. Do not add real patient data or password files to Git. No other user action is required for a coursework submission.
