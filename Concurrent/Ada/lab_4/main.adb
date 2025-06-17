-- Main.adb
-- Main program to demonstrate Szymanski's algorithm

with Szymanski_Algorithm; use Szymanski_Algorithm;
with Ada.Text_IO; use Ada.Text_IO;

procedure Main is

   -- Shared flags object
   Shared_Flags : aliased Atomic_Flags;
   
   -- Process tasks
   Process_1 : Process_Task(1, Shared_Flags'Access);
   Process_2 : Process_Task(2, Shared_Flags'Access);
   Process_3 : Process_Task(3, Shared_Flags'Access);

begin
   Put_Line("=== Szymanski's Mutual Exclusion Algorithm Demo ===");
   Put_Line("Process States:");
   Put_Line("  Local_Section     - Process doing local work");
   Put_Line("  Entry_Protocol_1  - f[i] = 1");
   Put_Line("  Entry_Protocol_2  - f[i] = 2, await condition");
   Put_Line("  Entry_Protocol_3  - f[i] = 3, await condition");
   Put_Line("  Entry_Protocol_4  - f[i] = 4, await condition");
   Put_Line("  Critical_Section  - Process in critical section");
   Put_Line("  Exit_Protocol     - f[i] = 0, exiting");
   Put_Line("");
   Put_Line("Flag Values: 0=idle, 1=want_to_enter, 2=waiting, 3=ready, 4=entering");
   Put_Line("=======================================================");
   Put_Line("");
   
   -- Tasks will start automatically
   -- Main program waits for all tasks to complete
   
end Main;