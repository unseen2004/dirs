with Ada.Text_IO; use Ada.Text_IO;
with Ada.Numerics.Float_Random; use Ada.Numerics.Float_Random;
with Random_Seeds; use Random_Seeds;
with Ada.Real_Time; use Ada.Real_Time;
with Ada.Calendar;

procedure Ex_2_Enhanced is

-- Travelers moving on the board
  Nr_Of_Travelers : constant Integer := 8; -- Reduced for better wild tenant interaction
  
  Min_Steps : constant Integer := 15;
  Max_Steps : constant Integer := 50;
  
  Min_Delay : constant Duration := 0.02;
  Max_Delay : constant Duration := 0.08;
  
  -- **DEADLOCK DETECTION**: Timeout for movement attempts
  Move_Timeout : constant Duration := 5.0 * Max_Delay;

-- 2D Board with torus topology
  Board_Width  : constant Integer := 10;  -- Smaller board for more interaction
  Board_Height : constant Integer := 10;

-- **WILD TENANT PARAMETERS**
  Wild_Tenant_Spawn_Probability : constant Float := 0.02;  -- 2% chance per check
  Wild_Tenant_Check_Interval : constant Duration := 0.1;   -- Check every 100ms
  Wild_Tenant_Min_Lifetime : constant Duration := 2.0;     -- Minimum 2 seconds
  Wild_Tenant_Max_Lifetime : constant Duration := 8.0;     -- Maximum 8 seconds

-- Timing
  Start_Time : Ada.Real_Time.Time := Ada.Real_Time.Clock;

-- Random seeds
  Seeds : Seed_Array_Type(1..Nr_Of_Travelers + Board_Width * Board_Height) := 
    Make_Seeds(Nr_Of_Travelers + Board_Width * Board_Height);

-- **CORE TYPES AND STRUCTURES**

  -- Positions on the board
  type Position_Type is record    
    X: Integer range 0 .. Board_Width - 1; 
    Y: Integer range 0 .. Board_Height - 1; 
  end record;
  
  -- **FIX**: Define named array type for adjacent positions
  type Adjacent_Positions_Type is array(1..4) of Position_Type;
  
  -- **OCCUPANT TYPES**: Distinguishes between travelers and wild tenants
  type Occupant_Type is (Empty, Traveler, Wild_Tenant);
  
  -- Movement request result
  type Move_Result_Type is (Success, Blocked, Evicted_Tenant);

  -- **TRACE SYSTEM**: Records all entity movements for visualization
  type Trace_Type is record         
    Time_Stamp: Duration;         
    Id : Integer;
    Position: Position_Type;      
    Symbol: Character;         
  end record;         

  type Trace_Array_type is array(0 .. Max_Steps * 2) of Trace_Type; -- Extra space for wild tenants

  type Traces_Sequence_Type is record
    Last: Integer := -1;
    Trace_Array: Trace_Array_type;
  end record; 

  -- **FIELD MANAGEMENT TASK**: Each field managed independently
  task type Field_Task_Type is
    entry Initialize(X, Y: Integer; Seed: Integer);
    entry Request_Move(From_Pos: Position_Type; 
                      Traveler_Id: Integer; 
                      Traveler_Symbol: Character;
                      Result: out Move_Result_Type);
    entry Try_Spawn_Wild_Tenant;
    entry Evict_Wild_Tenant(New_Pos: out Position_Type; Success: out Boolean);
    entry Release_Field(Occupant: Occupant_Type);
  end Field_Task_Type;

  -- **BOARD STRUCTURE**: 2D array of field tasks
  type Board_Type is array(0 .. Board_Width-1, 0 .. Board_Height-1) of Field_Task_Type;
  Board : Board_Type;

-- **UTILITY FUNCTIONS**: Movement calculations with torus topology

  -- **FIX**: Use named array type in function signature
  function Get_Adjacent_Positions(Pos: Position_Type) return Adjacent_Positions_Type is
    Result : Adjacent_Positions_Type;
  begin
    -- Up, Down, Left, Right with torus wrapping
    Result(1) := (Pos.X, (Pos.Y + Board_Height - 1) mod Board_Height);  -- Up
    Result(2) := (Pos.X, (Pos.Y + 1) mod Board_Height);                 -- Down  
    Result(3) := ((Pos.X + Board_Width - 1) mod Board_Width, Pos.Y);    -- Left
    Result(4) := ((Pos.X + 1) mod Board_Width, Pos.Y);                  -- Right
    return Result;
  end Get_Adjacent_Positions;

  procedure Move_Up(Position: in out Position_Type) is
  begin
    Position.Y := (Position.Y + Board_Height - 1) mod Board_Height;
  end Move_Up;

  procedure Move_Down(Position: in out Position_Type) is
  begin
    Position.Y := (Position.Y + 1) mod Board_Height;
  end Move_Down;

  procedure Move_Right(Position: in out Position_Type) is
  begin
    Position.X := (Position.X + 1) mod Board_Width;
  end Move_Right;

  procedure Move_Left(Position: in out Position_Type) is
  begin
    Position.X := (Position.X + Board_Width - 1) mod Board_Width;
  end Move_Left;

-- **TRACE MANAGEMENT**: Printing and collection

  procedure Print_Trace(Trace : Trace_Type) is
  begin
    Put_Line(
        Duration'Image(Trace.Time_Stamp) & " " &
        Integer'Image(Trace.Id) & " " &
        Integer'Image(Trace.Position.X) & " " &
        Integer'Image(Trace.Position.Y) & " " &
        (' ', Trace.Symbol)
      );
  end Print_Trace;

  procedure Print_Traces(Traces : Traces_Sequence_Type) is
  begin
    for I in 0 .. Traces.Last loop
      Print_Trace(Traces.Trace_Array(I));
    end loop;
  end Print_Traces;

  -- **PRINTER TASK**: Collects and outputs all traces
  task Printer is
    entry Report(Traces : Traces_Sequence_Type);
  end Printer;
  
  task body Printer is 
    Reports_Received: Integer := 0;
    Expected_Reports: Integer := Nr_Of_Travelers + Board_Width * Board_Height; -- Travelers + Fields
  begin
    loop
      select
        accept Report(Traces : Traces_Sequence_Type) do
          Print_Traces(Traces);
          Reports_Received := Reports_Received + 1;
        end Report;
      or
        when Reports_Received >= Nr_Of_Travelers => -- Allow termination when travelers done
          terminate;
      end select;
    end loop;
  end Printer;

-- **FIELD TASK IMPLEMENTATION**: Core synchronization and wild tenant management

  task body Field_Task_Type is
    My_Position : Position_Type;
    Current_Occupant : Occupant_Type := Empty;
    Occupant_Id : Integer := -1;
    Occupant_Symbol : Character := ' ';
    
    -- **WILD TENANT STATE**
    Wild_Tenant_Generator : Generator;
    Wild_Tenant_Id : Integer;
    Wild_Tenant_Lifetime : Duration;
    Wild_Tenant_Spawn_Time : Ada.Calendar.Time;
    Wild_Tenant_Traces : Traces_Sequence_Type;
    
    procedure Store_Wild_Tenant_Trace(Symbol: Character) is
      Time_Stamp : Duration := To_Duration(Ada.Real_Time.Clock - Start_Time);
    begin  
      Wild_Tenant_Traces.Last := Wild_Tenant_Traces.Last + 1;
      Wild_Tenant_Traces.Trace_Array(Wild_Tenant_Traces.Last) := ( 
          Time_Stamp => Time_Stamp,
          Id => Wild_Tenant_Id,
          Position => My_Position,
          Symbol => Symbol
        );
    end Store_Wild_Tenant_Trace;
    
    -- **EVICTION PROTOCOL**: Relocate wild tenant to adjacent empty field
    procedure Try_Evict_Wild_Tenant(Success: out Boolean; New_Pos: out Position_Type) is
      Adjacent : Adjacent_Positions_Type := Get_Adjacent_Positions(My_Position);
      Move_Result : Move_Result_Type;
    begin
      Success := False;
      
      -- Try each adjacent position
      for I in Adjacent'Range loop
        select
          Board(Adjacent(I).X, Adjacent(I).Y).Request_Move(
            My_Position, Wild_Tenant_Id, Occupant_Symbol, Move_Result);
          
          if Move_Result = Success then
            New_Pos := Adjacent(I);
            Success := True;
            
            -- **TRACE WILD TENANT MOVEMENT**
            Store_Wild_Tenant_Trace(Occupant_Symbol);
            exit;
          end if;
        or
          delay 0.001; -- Brief timeout for non-blocking check
        end select;
      end loop;
    end Try_Evict_Wild_Tenant;
    
  begin
    accept Initialize(X, Y: Integer; Seed: Integer) do
      My_Position := (X, Y);
      Reset(Wild_Tenant_Generator, Seed);
      Wild_Tenant_Id := 1000 + X * Board_Width + Y; -- Unique ID for wild tenants
    end Initialize;
    
    -- **MAIN FIELD LOOP**: Handle requests and manage wild tenants
    loop
      select
        -- **MOVEMENT REQUEST HANDLING**
        accept Request_Move(From_Pos: Position_Type; 
                           Traveler_Id: Integer; 
                           Traveler_Symbol: Character;
                           Result: out Move_Result_Type) do
          case Current_Occupant is
            when Empty =>
              Current_Occupant := Traveler;
              Occupant_Id := Traveler_Id;
              Occupant_Symbol := Traveler_Symbol;
              Result := Success;
              
            when Wild_Tenant =>
              -- **EVICTION ATTEMPT**: Try to relocate wild tenant
              declare
                Eviction_Success : Boolean;
                New_Tenant_Pos : Position_Type;
              begin
                Try_Evict_Wild_Tenant(Eviction_Success, New_Tenant_Pos);
                
                if Eviction_Success then
                  -- Wild tenant moved, traveler can occupy
                  Current_Occupant := Traveler;
                  Occupant_Id := Traveler_Id;
                  Occupant_Symbol := Traveler_Symbol;
                  Result := Evicted_Tenant;
                else
                  -- No space for wild tenant, movement blocked
                  Result := Blocked;
                end if;
              end;
              
            when Traveler =>
              Result := Blocked; -- Field occupied by another traveler
          end case;
        end Request_Move;
        
      or
        -- **WILD TENANT SPAWNING**: Random appearance on empty fields
        accept Try_Spawn_Wild_Tenant do
          if Current_Occupant = Empty and then 
             Random(Wild_Tenant_Generator) < Wild_Tenant_Spawn_Probability then
            
            Current_Occupant := Wild_Tenant;
            Occupant_Symbol := Character'Val(Character'Pos('0') + 
              Integer(Float'Floor(10.0 * Random(Wild_Tenant_Generator))));
            
            -- **SET WILD TENANT LIFETIME**
            Wild_Tenant_Lifetime := Wild_Tenant_Min_Lifetime + 
              Duration(Random(Wild_Tenant_Generator)) * 
              (Wild_Tenant_Max_Lifetime - Wild_Tenant_Min_Lifetime);
            Wild_Tenant_Spawn_Time := Ada.Calendar.Clock;
            
            -- Record spawn trace
            Store_Wild_Tenant_Trace(Occupant_Symbol);
          end if;
        end Try_Spawn_Wild_Tenant;
        
      or
        -- **FIELD RELEASE**: Clear occupant
        accept Release_Field(Occupant: Occupant_Type) do
          if Current_Occupant = Occupant then
            Current_Occupant := Empty;
            Occupant_Id := -1;
            Occupant_Symbol := ' ';
          end if;
        end Release_Field;
        
      or
        -- **WILD TENANT EVICTION REQUEST**
        when Current_Occupant = Wild_Tenant =>
          accept Evict_Wild_Tenant(New_Pos: out Position_Type; Success: out Boolean) do
            Try_Evict_Wild_Tenant(Success, New_Pos);
            if Success then
              Current_Occupant := Empty;
            end if;
          end Evict_Wild_Tenant;
          
      or
        -- **WILD TENANT LIFETIME CHECK**: Remove expired tenants
        when Current_Occupant = Wild_Tenant =>
          delay Wild_Tenant_Check_Interval;
          
          declare
            Current_Time : Ada.Calendar.Time := Ada.Calendar.Clock;
            Elapsed : Duration := Ada.Calendar."-"(Current_Time, Wild_Tenant_Spawn_Time);
          begin
            if Elapsed >= Wild_Tenant_Lifetime then
              -- **WILD TENANT EXPIRY**: Record disappearance and clean up
              Wild_Tenant_Traces.Last := Wild_Tenant_Traces.Last + 1;
              Wild_Tenant_Traces.Trace_Array(Wild_Tenant_Traces.Last) := (
                Time_Stamp => To_Duration(Ada.Real_Time.Clock - Start_Time),
                Id => Wild_Tenant_Id,
                Position => (Board_Width, Board_Height), -- Off-board position
                Symbol => '.'
              );
              
              -- Report wild tenant traces before disappearing
              Printer.Report(Wild_Tenant_Traces);
              
              Current_Occupant := Empty;
              Wild_Tenant_Traces.Last := -1; -- Reset for potential future tenant
            end if;
          end;
          
      or
        terminate; -- Allow termination when simulation ends
      end select;
    end loop;
  end Field_Task_Type;

-- **WILD TENANT SPAWNER**: Periodically triggers spawn attempts across board

  task Wild_Tenant_Spawner is
  end Wild_Tenant_Spawner;
  
  task body Wild_Tenant_Spawner is
  begin
    loop
      select
        delay Wild_Tenant_Check_Interval;
        
        -- **SPAWN CYCLE**: Check all fields for potential wild tenant spawning
        for X in 0 .. Board_Width - 1 loop
          for Y in 0 .. Board_Height - 1 loop
            select
              Board(X, Y).Try_Spawn_Wild_Tenant;
            or
              delay 0.001; -- Non-blocking attempt
            end select;
          end loop;
        end loop;
      or
        terminate;
      end select;
    end loop;
  end Wild_Tenant_Spawner;

-- **TRAVELER IMPLEMENTATION**: Enhanced with field-based movement requests

  type Traveler_Type is record
    Id: Integer;
    Symbol: Character;
    Position: Position_Type;    
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
    
    -- **ENHANCED MOVEMENT**: Request-based system with eviction handling
    procedure Make_Step is
      N : Integer; 
      Target_Pos : Position_Type := Traveler.Position;
      Move_Result : Move_Result_Type;
      Attempt_Start_Time : Ada.Calendar.Time;
      Success : Boolean := False;
    begin
      -- Release current field
      Board(Traveler.Position.X, Traveler.Position.Y).Release_Field(Traveler);
      
      -- Choose random direction
      N := Integer(Float'Floor(4.0 * Random(G)));
      
      case N is
        when 0 => Move_Up(Target_Pos);
        when 1 => Move_Down(Target_Pos);
        when 2 => Move_Left(Target_Pos);
        when 3 => Move_Right(Target_Pos);
        when others => Put_Line("Invalid direction: " & Integer'Image(N));
      end case;
      
      -- **MOVEMENT ATTEMPT WITH TIMEOUT**: Prevent infinite blocking
      Attempt_Start_Time := Ada.Calendar.Clock;
      
      loop
        select
          Board(Target_Pos.X, Target_Pos.Y).Request_Move(
            Traveler.Position, Traveler.Id, Traveler.Symbol, Move_Result);
          
          case Move_Result is
            when Success | Evicted_Tenant =>
              Traveler.Position := Target_Pos;
              Success := True;
              exit;
              
            when Blocked =>
              null; -- Try again
          end case;
          
        or
          delay 0.005; -- Brief delay between attempts
        end select;
        
        -- **DEADLOCK DETECTION**: Check for timeout
        declare
          Current_Time : Ada.Calendar.Time := Ada.Calendar.Clock;
          Elapsed : Duration := Ada.Calendar."-"(Current_Time, Attempt_Start_Time);
        begin
          if Elapsed > Move_Timeout then
            -- **DEADLOCK RECOVERY**: Return to original position and mark as deadlocked
            Board(Traveler.Position.X, Traveler.Position.Y).Request_Move(
              Target_Pos, Traveler.Id, Traveler.Symbol, Move_Result);
            
            -- Change to lowercase to indicate deadlock
            Traveler.Symbol := Character'Val(Character'Pos(Traveler.Symbol) + 32);
            Deadlock_Detected := True;
            exit;
          end if;
        end;
      end loop;
    end Make_Step;

  begin
    accept Init(Id: Integer; Seed: Integer; Symbol: Character) do
      Reset(G, Seed); 
      Traveler.Id := Id;
      Traveler.Symbol := Symbol;
      
      -- **INITIAL PLACEMENT**: Find empty field for starting position
      loop
        Traveler.Position := (
          X => Integer(Float'Floor(Float(Board_Width) * Random(G))),
          Y => Integer(Float'Floor(Float(Board_Height) * Random(G)))
        );
        
        declare
          Move_Result : Move_Result_Type;
        begin
          Board(Traveler.Position.X, Traveler.Position.Y).Request_Move(
            Traveler.Position, Traveler.Id, Traveler.Symbol, Move_Result);
          exit when Move_Result = Success;
        end;
      end loop;
      
      Store_Trace; -- Record starting position
      
      Nr_of_Steps := Min_Steps + Integer(Float(Max_Steps - Min_Steps) * Random(G));
      Time_Stamp := To_Duration(Ada.Real_Time.Clock - Start_Time);
    end Init;
    
    accept Start do
      null;
    end Start;

    -- **MOVEMENT LOOP**: Execute planned steps until completion or deadlock
    for Step in 0 .. Nr_of_Steps loop
      exit when Deadlock_Detected;
      
      delay Min_Delay + (Max_Delay - Min_Delay) * Duration(Random(G));
      
      Make_Step;
      Store_Trace;
      Time_Stamp := To_Duration(Ada.Real_Time.Clock - Start_Time);
    end loop;
    
    -- **CLEANUP**: Release field and report traces
    Board(Traveler.Position.X, Traveler.Position.Y).Release_Field(Traveler);
    Printer.Report(Traces);
  end Traveler_Task_Type;

-- **MAIN PROGRAM VARIABLES**
  Travel_Tasks: array(0 .. Nr_Of_Travelers-1) of Traveler_Task_Type;
  Symbol : Character := 'A';
  
begin 
  -- **INITIALIZATION OUTPUT**: Parameters for visualization script
  Put_Line(
      "-1 " &
      Integer'Image(Nr_Of_Travelers) & " " &
      Integer'Image(Board_Width) & " " &
      Integer'Image(Board_Height)      
    );

  -- **BOARD INITIALIZATION**: Setup field tasks with unique seeds
  for X in 0 .. Board_Width - 1 loop
    for Y in 0 .. Board_Height - 1 loop
      Board(X, Y).Initialize(X, Y, Seeds(Nr_Of_Travelers + 1 + X * Board_Width + Y));
    end loop;
  end loop;

  -- **TRAVELER INITIALIZATION**: Setup traveler tasks
  for I in Travel_Tasks'Range loop
    Travel_Tasks(I).Init(I, Seeds(I+1), Symbol);
    Symbol := Character'Succ(Symbol);
  end loop;

  -- **SIMULATION START**: Begin all traveler movements
  for I in Travel_Tasks'Range loop
    Travel_Tasks(I).Start;
  end loop;

end Ex_2_Enhanced;
