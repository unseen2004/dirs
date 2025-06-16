with Ada.Text_IO; use Ada.Text_IO;
with Ada.Numerics.Float_Random; use Ada.Numerics.Float_Random;
with Random_Seeds; use Random_Seeds;
with Ada.Real_Time; use Ada.Real_Time;

procedure  Mutex_Template is

-- Processes 

  Nr_Of_Processes : constant Integer :=15;

  Min_Steps : constant Integer := 50 ;
  Max_Steps : constant Integer := 100 ;

  Min_Delay : constant Duration := 0.01;
  Max_Delay : constant Duration := 0.05;

-- States of a Process 

  type Process_State is (
    Local_Section,
    Entry_Protocol,
    Critical_Section,
    Exit_Protocol
    );

-- 2D Board display board

  Board_Width  : constant Integer := Nr_Of_Processes;
  Board_Height : constant Integer := Process_State'Pos( Process_State'Last ) + 1;

-- Timing

  Start_Time : Time := Clock;  -- global startnig time

-- Random seeds for the tasks' random number generators
 
  Seeds : Seed_Array_Type( 1..Nr_Of_Processes ) := Make_Seeds( Nr_Of_Processes );

-- Bakery Algorithm Variables
  type Ticket_Array is array (0 .. Nr_Of_Processes-1) of Integer;
  type Choosing_Array is array (0 .. Nr_Of_Processes-1) of Boolean;
  
  Tickets : Ticket_Array := (others => 0);
  Choosing : Choosing_Array := (others => False);

-- Protected object to track maximum ticket used
  protected Max_Ticket_Tracker is
    procedure Update_Max(New_Ticket : Integer);
    function Get_Max return Integer;
  private
    Max_Ticket : Integer := 0;
  end Max_Ticket_Tracker;

  protected body Max_Ticket_Tracker is
    procedure Update_Max(New_Ticket : Integer) is
    begin
      if New_Ticket > Max_Ticket then
        Max_Ticket := New_Ticket;
      end if;
    end Update_Max;
    
    function Get_Max return Integer is
    begin
      return Max_Ticket;
    end Get_Max;
  end Max_Ticket_Tracker;

-- Types, procedures and functions

  -- Postitions on the board
  type Position_Type is record	
    X: Integer range 0 .. Board_Width - 1; 
    Y: Integer range 0 .. Board_Height - 1; 
  end record;	   

  -- traces of Processes
  type Trace_Type is record 	      
    Time_Stamp:  Duration;	      
    Id : Integer;
    Position: Position_Type;      
    Symbol: Character;	      
  end record;	      

  type Trace_Array_type is  array(0 .. Max_Steps) of Trace_Type;

  type Traces_Sequence_Type is record
    Last: Integer := -1;
    Trace_Array: Trace_Array_type ;
  end record; 

  procedure Print_Trace( Trace : Trace_Type ) is
    Symbol : String := ( ' ', Trace.Symbol );
  begin
    Put_Line(
        Duration'Image( Trace.Time_Stamp ) & " " &
        Integer'Image( Trace.Id ) & " " &
        Integer'Image( Trace.Position.X ) & " " &
        Integer'Image( Trace.Position.Y ) & " " &
        ( ' ', Trace.Symbol ) -- print as string to avoid: '
      );
  end Print_Trace;

  procedure Print_Traces( Traces : Traces_Sequence_Type ) is
  begin
    for I in 0 .. Traces.Last loop
      Print_Trace( Traces.Trace_Array( I ) );
    end loop;
  end Print_Traces;

  -- task Printer collects and prints reports of traces and the line with the parameters

  task Printer is
    entry Report( Traces : Traces_Sequence_Type );
  end Printer;
  
  task body Printer is 
  begin
  
    -- Collect and print the traces
    
    for I in 1 .. Nr_Of_Processes loop -- range for TESTS !!!
        accept Report( Traces : Traces_Sequence_Type ) do
          -- Put_Line("I = " & I'Image );
          Print_Traces( Traces );
        end Report;
      end loop;

    -- Print the line with the parameters needed for display script:

    Put(
      "-1 "&
      Integer'Image( Nr_Of_Processes ) &" "&
      Integer'Image( Board_Width ) &" "&
      Integer'Image( Board_Height ) &" "       
    );
    for I in Process_State'Range loop
      Put( I'Image &";" );
    end loop;
    Put_Line("MAX_TICKET=" & Integer'Image(Max_Ticket_Tracker.Get_Max) & ";"); -- Display max ticket used

  end Printer;

  -- Processes
  type Process_Type is record
    Id: Integer;
    Symbol: Character;
    Position: Position_Type;    
  end record;

  task type Process_Task_Type is	
    entry Init(Id: Integer; Seed: Integer; Symbol: Character);
    entry Start;
  end Process_Task_Type;	

  task body Process_Task_Type is
    G : Generator;
    Process : Process_Type;
    Time_Stamp : Duration;
    Nr_of_Steps: Integer;
    Traces: Traces_Sequence_Type; 
    My_Max_Ticket : Integer := 0; -- Track max ticket for this process

    procedure Store_Trace is
    begin  
      Traces.Last := Traces.Last + 1;
      Traces.Trace_Array( Traces.Last ) := ( 
          Time_Stamp => Time_Stamp,
          Id => Process.Id,
          Position => Process.Position,
          Symbol => Process.Symbol
        );
    end Store_Trace;

    procedure Change_State( State: Process_State ) is
    begin
      Time_Stamp := To_Duration ( Clock - Start_Time ); -- reads global clock
      Process.Position.Y := Process_State'Pos( State );
      Store_Trace;
    end;

    -- Bakery Algorithm Helper Functions
    function Max_Ticket_Number return Integer is
      Max_Ticket : Integer := 0;
    begin
      for I in Tickets'Range loop
        if Tickets(I) > Max_Ticket then
          Max_Ticket := Tickets(I);
        end if;
      end loop;
      return Max_Ticket;
    end Max_Ticket_Number;
    
    function Has_Priority(My_Id : Integer; Other_Id : Integer) return Boolean is
    begin
      -- Priority comparison: (ticket, process_id)
      if Tickets(My_Id) < Tickets(Other_Id) then
        return True;
      elsif Tickets(My_Id) = Tickets(Other_Id) then
        return My_Id < Other_Id;
      else
        return False;
      end if;
    end Has_Priority;

  begin
    accept Init(Id: Integer; Seed: Integer; Symbol: Character) do
      Reset(G, Seed); 
      Process.Id := Id;
      Process.Symbol := Symbol;
      -- Initial position 
      Process.Position := (
          X => Id,
          Y => Process_State'Pos( LOCAL_SECTION )
        );
      -- Number of steps to be made by the Process  
      Nr_of_Steps := Min_Steps + Integer( Float(Max_Steps - Min_Steps) * Random(G));
      -- Time_Stamp of initialization
      Time_Stamp := To_Duration ( Clock - Start_Time ); -- reads global clock
      Store_Trace; -- store starting position
    end Init;
    
    -- wait for initialisations of the remaining tasks:
    accept Start do
      null;
    end Start;

    for Step in 0 .. Nr_of_Steps/4 - 1  loop  -- TEST !!!
      -- LOCAL_SECTION - start
      delay Min_Delay+(Max_Delay-Min_Delay)*Duration(Random(G));
      -- LOCAL_SECTION - end

      Change_State( ENTRY_PROTOCOL ); -- starting ENTRY_PROTOCOL
      
      -- BAKERY ENTRY PROTOCOL
      -- Step 1: Signal that we're choosing a ticket
      Choosing(Process.Id) := True;
      
      -- Step 2: Get a new ticket (one higher than current maximum)
      Tickets(Process.Id) := Max_Ticket_Number + 1;
      
      -- Update our local maximum and global maximum
      if Tickets(Process.Id) > My_Max_Ticket then
        My_Max_Ticket := Tickets(Process.Id);
      end if;
      
      -- Step 3: Finished choosing
      Choosing(Process.Id) := False;
      
      -- Step 4: Wait for our turn
      for Other_Process in Tickets'Range loop
        if Other_Process /= Process.Id then
          -- Wait until other process finishes choosing
          while Choosing(Other_Process) loop
            delay 0.001; -- Small delay to prevent busy waiting
          end loop;
          
          -- Wait until we have priority or other process has no ticket
          while Tickets(Other_Process) /= 0 and then
                not Has_Priority(Process.Id, Other_Process) loop
            delay 0.001; -- Small delay to prevent busy waiting
          end loop;
        end if;
      end loop;
      
      Change_State( CRITICAL_SECTION ); -- starting CRITICAL_SECTION

      -- CRITICAL_SECTION - start
      delay Min_Delay+(Max_Delay-Min_Delay)*Duration(Random(G));
      -- CRITICAL_SECTION - end

      Change_State( EXIT_PROTOCOL ); -- starting EXIT_PROTOCOL
      
      -- BAKERY EXIT PROTOCOL
      -- Release our ticket
      Tickets(Process.Id) := 0;
      
      Change_State( LOCAL_SECTION ); -- starting LOCAL_SECTION      
    end loop;
    
    -- Update global maximum before reporting
    Max_Ticket_Tracker.Update_Max(My_Max_Ticket);
    
    Printer.Report( Traces );
  end Process_Task_Type;

-- local for main task

  Process_Tasks: array (0 .. Nr_Of_Processes-1) of Process_Task_Type; -- for tests
  Symbol : Character := 'A';

begin 
  -- init travelers tasks
  for I in Process_Tasks'Range loop
    Process_Tasks(I).Init( I, Seeds(I+1), Symbol );   -- `Seeds(I+1)` is ugly :-(
    Symbol := Character'Succ( Symbol );
  end loop;

  -- start travelers tasks
  for I in Process_Tasks'Range loop
    Process_Tasks(I).Start;
  end loop;

end Mutex_Template;
