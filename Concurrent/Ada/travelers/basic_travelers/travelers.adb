with Ada.Text_IO; use Ada.Text_IO;
with Ada.Numerics.Float_Random; use Ada.Numerics.Float_Random;
with Random_Seeds; use Random_Seeds;
with Ada.Real_Time; use Ada.Real_Time;
-- ADDED: Calendar for timeout handling
with Ada.Calendar; use Ada.Calendar;

procedure Travelers is

-- Travelers moving on the board
  Nr_Of_Travelers : constant Integer := 15;

  Min_Steps : constant Integer := 10;
  Max_Steps : constant Integer := 100;

  Min_Delay : constant Duration := 0.01;
  Max_Delay : constant Duration := 0.05;

  -- ADDED: Timeout for deadlock detection (slightly longer than maximum delay)
  Field_Wait_Timeout : constant Duration := 3.0 * Max_Delay;

-- 2D Board with torus topology
  Board_Width  : constant Integer := 15;
  Board_Height : constant Integer := 15;

-- Timing
  Start_Time : Time := Clock;  -- global starting time

-- Random seeds for the tasks' random number generators
  Seeds : Seed_Array_Type(1..Nr_Of_Travelers) := Make_Seeds(Nr_Of_Travelers);

-- Types, procedures and functions

  -- Positions on the board
  type Position_Type is record
    X: Integer range 0 .. Board_Width - 1;
    Y: Integer range 0 .. Board_Height - 1;
  end record;

  -- ADDED: Protected object to manage field occupancy
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

  -- ADDED: Board of fields with protected access
  type Board_Type is array(0 .. Board_Width-1, 0 .. Board_Height-1) of Field_Type;
  Board : Board_Type;

  -- elementary steps (unchanged)
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

  -- traces of travelers (unchanged)
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
  -- MODIFIED: Added ability to terminate when done
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
        -- ADDED: Allow termination when all travelers have reported
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
  end record;

  -- MODIFIED: Added terminate alternative
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

    -- MODIFIED: Make_Step now tries to occupy field
    procedure Make_Step is
      N : Integer;
      Success : Boolean := False;
      Target_Pos : Position_Type := Traveler.Position;
      Attempt_Start_Time : Ada.Calendar.Time;
      Waited_Too_Long : Boolean := False;
    begin
      -- Release current field before attempting to move
      Board(Traveler.Position.X, Traveler.Position.Y).Release;

      -- Choose random direction
      N := Integer(Float'Floor(4.0 * Random(G)));

      -- Determine target position
      case N is
        when 0 =>
          Move_Up(Target_Pos);
        when 1 =>
          Move_Down(Target_Pos);
        when 2 =>
          Move_Left(Target_Pos);
        when 3 =>
          Move_Right(Target_Pos);
        when others =>
          Put_Line(" ?????????????? " & Integer'Image(N));
      end case;

      -- Try to occupy target field with timeout
      Attempt_Start_Time := Ada.Calendar.Clock;

      loop
        Board(Target_Pos.X, Target_Pos.Y).Try_Occupy(Success);
        exit when Success;

        -- Check for potential deadlock
        if Ada.Calendar.Clock - Attempt_Start_Time > Field_Wait_Timeout then
          Waited_Too_Long := True;
          exit;
        end if;

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

      -- MODIFIED: Ensure initial position is unoccupied
      loop
        -- Random initial position:
        Traveler.Position := (
          X => Integer(Float'Floor(Float(Board_Width) * Random(G))),
          Y => Integer(Float'Floor(Float(Board_Height) * Random(G)))
        );

        declare
          Success : Boolean;
        begin
          Board(Traveler.Position.X, Traveler.Position.Y).Try_Occupy(Success);
          exit when Success;
        end;
      end loop;

      Store_Trace; -- store starting position

      -- Number of steps to be made by the traveler
      Nr_of_Steps := Min_Steps + Integer(Float(Max_Steps - Min_Steps) * Random(G));

      -- Time_Stamp of initialization
      Time_Stamp := To_Duration(Clock - Start_Time); -- reads global clock
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
      Time_Stamp := To_Duration(Clock - Start_Time); -- reads global clock
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

end Travelers;