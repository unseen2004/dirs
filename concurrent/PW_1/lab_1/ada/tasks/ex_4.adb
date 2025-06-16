with Ada.Text_IO; use Ada.Text_IO;
with Ada.Numerics.Float_Random; use Ada.Numerics.Float_Random;
with Random_Seeds; use Random_Seeds;
with Ada.Real_Time; use Ada.Real_Time;
with Ada.Calendar;

procedure Ex_4 is

-- Travelers moving on the board
  Nr_Of_Travelers : constant Integer := 15;

  Min_Steps : constant Integer := 10;
  Max_Steps : constant Integer := 100;

  Min_Delay : constant Duration := 0.01;
  Max_Delay : constant Duration := 0.05;
  
  -- Timeout for deadlock detection
  Field_Wait_Timeout : constant Duration := 3.0 * Max_Delay;

-- 2D Board with torus topology - must be equal to Nr_Of_Travelers for this exercise
  Board_Width  : constant Integer := 15;
  Board_Height : constant Integer := 15;

-- Timing
  Start_Time : Ada.Real_Time.Time := Ada.Real_Time.Clock;  -- global starting time

-- Random seeds for the tasks' random number generators
  Seeds : Seed_Array_Type(1..Nr_Of_Travelers) := Make_Seeds(Nr_Of_Travelers);

  -- Direction type to represent movement patterns
  type Direction_Type is (Up, Down, Left, Right);

-- Types, procedures and functions

  -- Positions on the board
  type Position_Type is record    
    X: Integer range 0 .. Board_Width - 1; 
    Y: Integer range 0 .. Board_Height - 1; 
  end record;     

  -- Protected object to manage field occupancy
  protected type Field_Type is
    -- Try to occupy a field, returns True if successful
    entry Try_Occupy(Success: out Boolean);
    -- Release a field
    procedure Release;
  private
    Occupied: Boolean := False;
  end Field_Type;

  protected body Field_Type is
    entry Try_Occupy(Success: out Boolean) when True is
    begin
      if Occupied then
        Success := False;
      else
        Occupied := True;
        Success := True;
      end if;
    end Try_Occupy;

    procedure Release is
    begin
      Occupied := False;
    end Release;
  end Field_Type;

  -- Board of fields with protected access
  type Board_Type is array(0 .. Board_Width-1, 0 .. Board_Height-1) of Field_Type;
  Board : Board_Type;

  -- elementary steps
  procedure Move_Down(Position: in out Position_Type) is
  begin
    Position.Y := (Position.Y + 1) mod Board_Height;
  end Move_Down;

  procedure Move_Up(Position: in out Position_Type) is
  begin
    Position.Y := (Position.Y + Board_Height - 1) mod Board_Height;
  end Move_Up;

  procedure Move_Right(Position: in out Position_Type) is
  begin
    Position.X := (Position.X + 1) mod Board_Width;
  end Move_Right;

  procedure Move_Left(Position: in out Position_Type) is
  begin
    Position.X := (Position.X + Board_Width - 1) mod Board_Width;
  end Move_Left;

  -- traces of travelers
  type Trace_Type is record         
    Time_Stamp: Duration;         
    Id : Integer;
    Position: Position_Type;      
    Symbol: Character;         
  end record;         

  type Trace_Array_type is array(0 .. Max_Steps) of Trace_Type;

  type Traces_Sequence_Type is record
    Last: Integer := -1;
    Trace_Array: Trace_Array_type;
  end record; 

  procedure Print_Trace(Trace : Trace_Type) is
    Symbol : String := (' ', Trace.Symbol);
  begin
    Put_Line(
        Duration'Image(Trace.Time_Stamp) & " " &
        Integer'Image(Trace.Id) & " " &
        Integer'Image(Trace.Position.X) & " " &
        Integer'Image(Trace.Position.Y) & " " &
        (' ', Trace.Symbol) -- print as string to avoid: '
      );
  end Print_Trace;

  procedure Print_Traces(Traces : Traces_Sequence_Type) is
  begin
    for I in 0 .. Traces.Last loop
      Print_Trace(Traces.Trace_Array(I));
    end loop;
  end Print_Traces;

  -- task Printer collects and prints reports of traces
  task Printer is
    entry Report(Traces : Traces_Sequence_Type);
  end Printer;
  
  task body Printer is 
    Travelers_Reported: Integer := 0;
  begin
    loop
      select
        accept Report(Traces : Traces_Sequence_Type) do
          Print_Traces(Traces);
          Travelers_Reported := Travelers_Reported + 1;
        end Report;
      or
        -- Allow termination when all travelers have reported
        when Travelers_Reported = Nr_Of_Travelers =>
          terminate;
      end select;
    end loop;
  end Printer;

  -- travelers
  type Traveler_Type is record
    Id: Integer;
    Symbol: Character;
    Position: Position_Type;    
    Direction: Direction_Type;  -- Added fixed direction
  end record;

  task type Traveler_Task_Type is    
    entry Init(Id: Integer; Seed: Integer; Symbol: Character);
    entry Start;
  end Traveler_Task_Type;    

  task body Traveler_Task_Type is
    G : Generator;
    Traveler : Traveler_Type;
    Time_Stamp : Duration;
    Nr_of_Steps: Integer;
    Traces: Traces_Sequence_Type; 
    Deadlock_Detected : Boolean := False;

    procedure Store_Trace is
    begin  
      Traces.Last := Traces.Last + 1;
      Traces.Trace_Array(Traces.Last) := ( 
          Time_Stamp => Time_Stamp,
          Id => Traveler.Id,
          Position => Traveler.Position,
          Symbol => Traveler.Symbol
        );
    end Store_Trace;
    
    -- Modified to use fixed direction
    procedure Make_Step is
      Success : Boolean := False;
      Target_Pos : Position_Type := Traveler.Position;
      Attempt_Start_Time : Ada.Calendar.Time;
      Waited_Too_Long : Boolean := False;
    begin
      -- Release current field before attempting to move
      Board(Traveler.Position.X, Traveler.Position.Y).Release;
      
      -- Use traveler's fixed direction
      case Traveler.Direction is
        when Up =>
          Move_Up(Target_Pos);
        when Down =>
          Move_Down(Target_Pos);
        when Left =>
          Move_Left(Target_Pos);
        when Right =>
          Move_Right(Target_Pos);
      end case;
      
      -- Try to occupy target field with timeout
      Attempt_Start_Time := Ada.Calendar.Clock;
      
      loop
        Board(Target_Pos.X, Target_Pos.Y).Try_Occupy(Success);
        exit when Success;
        
        -- Check for timeout (potential deadlock)
        declare
          Current_Time : Ada.Calendar.Time := Ada.Calendar.Clock;
          Elapsed : Duration;
        begin
          Elapsed := Ada.Calendar."-"(Current_Time, Attempt_Start_Time);
          if Elapsed > Field_Wait_Timeout then
            Waited_Too_Long := True;
            exit;
          end if;
        end;
        
        -- Small delay before retrying
        delay 0.001;
      end loop;
      
      if Waited_Too_Long then
        -- Potential deadlock detected - reoccupy original position
        Board(Traveler.Position.X, Traveler.Position.Y).Try_Occupy(Success);
        
        -- Change symbol to lowercase to indicate deadlock
        Traveler.Symbol := Character'Val(Character'Pos(Traveler.Symbol) + 32);
        Deadlock_Detected := True;
      else
        -- Successful move
        Traveler.Position := Target_Pos;
      end if;
    end Make_Step;

  begin
    accept Init(Id: Integer; Seed: Integer; Symbol: Character) do
      Reset(G, Seed); 
      Traveler.Id := Id;
      Traveler.Symbol := Symbol;
      
      -- MODIFIED: Initialize position on diagonal (i,i)
      Traveler.Position := (
        X => Id,
        Y => Id
      );
      
      -- Try to occupy the initial position - should always succeed
      declare
        Success : Boolean;
      begin
        Board(Traveler.Position.X, Traveler.Position.Y).Try_Occupy(Success);
        if not Success then
          -- This should never happen in this scenario, but just in case
          Put_Line("ERROR: Could not occupy initial position for traveler " & Integer'Image(Id));
        end if;
      end;
      
      -- MODIFIED: Set fixed direction based on ID parity
      if Id mod 2 = 0 then
        -- Even ID: choose vertical direction (Up or Down)
        if Random(G) < 0.5 then
          Traveler.Direction := Up;
        else
          Traveler.Direction := Down;
        end if;
      else
        -- Odd ID: choose horizontal direction (Left or Right)
        if Random(G) < 0.5 then
          Traveler.Direction := Left;
        else
          Traveler.Direction := Right;
        end if;
      end if;
      
      Store_Trace; -- store starting position
      
      -- Number of steps to be made by the traveler  
      Nr_of_Steps := Min_Steps + Integer(Float(Max_Steps - Min_Steps) * Random(G));
      
      -- Time_Stamp of initialization
      Time_Stamp := To_Duration(Ada.Real_Time.Clock - Start_Time);
    end Init;
    
    -- wait for initialisations of the remaining tasks:
    accept Start do
      null;
    end Start;

    for Step in 0 .. Nr_of_Steps loop
      exit when Deadlock_Detected;
      
      delay Min_Delay + (Max_Delay - Min_Delay) * Duration(Random(G));
      
      -- do action ...
      Make_Step;
      Store_Trace;
      Time_Stamp := To_Duration(Ada.Real_Time.Clock - Start_Time);
    end loop;
    
    -- Report traces and terminate
    Printer.Report(Traces);
  end Traveler_Task_Type;

-- local for main task
  Travel_Tasks: array(0 .. Nr_Of_Travelers-1) of Traveler_Task_Type;
  Symbol : Character := 'A';
begin 
  -- Print the line with the parameters needed for display script:
  Put_Line(
      "-1 " &
      Integer'Image(Nr_Of_Travelers) & " " &
      Integer'Image(Board_Width) & " " &
      Integer'Image(Board_Height)      
    );

  -- init travelers tasks
  for I in Travel_Tasks'Range loop
    Travel_Tasks(I).Init(I, Seeds(I+1), Symbol);
    Symbol := Character'Succ(Symbol);
  end loop;

  -- start travelers tasks
  for I in Travel_Tasks'Range loop
    Travel_Tasks(I).Start;
  end loop;

end Ex_4;
