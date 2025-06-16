with Ada.Text_IO; use Ada.Text_IO;
with Ada.Integer_Text_IO; use Ada.Integer_Text_IO;
with Ada.Numerics.Discrete_Random;
with Ada.Containers.Vectors;
with Ada.Calendar; use Ada.Calendar;

procedure Extended_Travelers is

    -- Configuration constants
    Nr_Of_Travelers : constant Integer := 15;
    Min_Steps : constant Integer := 10;
    Max_Steps : constant Integer := 100;
    Min_Delay : constant Duration := 0.01;
    Max_Delay : constant Duration := 0.05;
    
    Board_Width : constant Integer := 15;
    Board_Height : constant Integer := 15;
    
    -- Wild settler configuration
    Wild_Settler_Min_Lifetime : constant Duration := 2.0;
    Wild_Settler_Max_Lifetime : constant Duration := 8.0;
    Wild_Settler_Spawn_Probability : constant Float := 0.001; -- Per field per time unit

    -- Types for coordinates and movement
    type Position is record
        X : Integer range 0 .. Board_Width - 1;
        Y : Integer range 0 .. Board_Height - 1;
    end record;
    
    type Direction is (North, South, East, West);
    
    -- Random number generation - fixed naming conflicts
    subtype Random_Range is Integer range 1 .. 1000;
    package Random_Int is new Ada.Numerics.Discrete_Random(Random_Range);
    
    subtype Direction_Range is Integer range 1 .. 4;
    package Random_Dir is new Ada.Numerics.Discrete_Random(Direction_Range);
    
    Gen : Random_Int.Generator;
    Dir_Gen : Random_Dir.Generator;
    
    -- Movement history record
    type Move_Record is record
        Time_Stamp : Integer;
        Id : Integer;
        X : Integer;
        Y : Integer;
        Symbol : Character;
    end record;
    
    package Move_Vector is new Ada.Containers.Vectors(Natural, Move_Record);
    use Move_Vector;
    
    -- Global time counter
    protected Time_Manager is
        function Get_Time return Integer;
        procedure Increment_Time;
    private
        Current_Time : Integer := 0;
    end Time_Manager;
    
    protected body Time_Manager is
        function Get_Time return Integer is
        begin
            return Current_Time;
        end Get_Time;
        
        procedure Increment_Time is
        begin
            Current_Time := Current_Time + 1;
        end Increment_Time;
    end Time_Manager;

    -- Field management - each field has its own protected object
    type Field_Content is (Empty, Traveler, Wild_Settler);
    
    protected type Field_Manager is
        entry Request_Entry(Traveler_Id : Integer; Success : out Boolean);
        entry Request_Wild_Entry(Settler_Id : Integer; Success : out Boolean);
        procedure Leave_Field;
        procedure Wild_Leave_Field;
        function Is_Empty return Boolean;
        function Get_Content return Field_Content;
        function Get_Occupant_Id return Integer;
        procedure Force_Wild_Move(Success : out Boolean);
    private
        Content : Field_Content := Empty;
        Occupant_Id : Integer := -1;
    end Field_Manager;
    
    protected body Field_Manager is
        entry Request_Entry(Traveler_Id : Integer; Success : out Boolean) when True is
        begin
            if Content = Empty then
                Content := Traveler;
                Occupant_Id := Traveler_Id;
                Success := True;
            else
                Success := False;
            end if;
        end Request_Entry;
        
        entry Request_Wild_Entry(Settler_Id : Integer; Success : out Boolean) when True is
        begin
            if Content = Empty then
                Content := Wild_Settler;
                Occupant_Id := Settler_Id;
                Success := True;
            else
                Success := False;
            end if;
        end Request_Wild_Entry;
        
        procedure Leave_Field is
        begin
            Content := Empty;
            Occupant_Id := -1;
        end Leave_Field;
        
        procedure Wild_Leave_Field is
        begin
            if Content = Wild_Settler then
                Content := Empty;
                Occupant_Id := -1;
            end if;
        end Wild_Leave_Field;
        
        procedure Force_Wild_Move(Success : out Boolean) is
        begin
            if Content = Wild_Settler then
                Content := Empty;
                Success := True;
            else
                Success := False;
            end if;
        end Force_Wild_Move;
        
        function Is_Empty return Boolean is
        begin
            return Content = Empty;
        end Is_Empty;
        
        function Get_Content return Field_Content is
        begin
            return Content;
        end Get_Content;
        
        function Get_Occupant_Id return Integer is
        begin
            return Occupant_Id;
        end Get_Occupant_Id;
    end Field_Manager;
    
    -- Array of field managers
    type Field_Array is array (0 .. Board_Width - 1, 0 .. Board_Height - 1) of Field_Manager;
    Fields : Field_Array;

    -- Printer task for collecting movement history
    task Printer is
        entry Print_Header;
        entry Add_Move(Move : Move_Record);
        entry Finish;
    end Printer;
    
    task body Printer is
        Moves : Vector;
        Done : Boolean := False;
    begin
        accept Print_Header do
            Put("-1 ");
            Put(Nr_Of_Travelers, 0);
            Put(" ");
            Put(Board_Width, 0);
            Put(" ");
            Put(Board_Height, 0);
            New_Line;
        end Print_Header;
        
        loop
            select
                accept Add_Move(Move : Move_Record) do
                    Append(Moves, Move);
                end Add_Move;
            or
                accept Finish do
                    Done := True;
                end Finish;
            end select;
            
            exit when Done;
        end loop;
        
        -- Print all collected moves
        for Move of Moves loop
            Put(Move.Time_Stamp, 0);
            Put(" ");
            Put(Move.Id, 0);
            Put(" ");
            Put(Move.X, 0);
            Put(" ");
            Put(Move.Y, 0);
            Put(" ");
            Put(Move.Symbol);
            New_Line;
        end loop;
    end Printer;

    -- Helper functions
    function Get_Next_Position(Pos : Position; Dir : Direction) return Position is
        Result : Position := Pos;
    begin
        case Dir is
            when North => Result.Y := (Pos.Y - 1 + Board_Height) mod Board_Height;
            when South => Result.Y := (Pos.Y + 1) mod Board_Height;
            when West  => Result.X := (Pos.X - 1 + Board_Width) mod Board_Width;
            when East  => Result.X := (Pos.X + 1) mod Board_Width;
        end case;
        return Result;
    end Get_Next_Position;
    
    function Random_Direction return Direction is
        Rand : Integer := Random_Dir.Random(Dir_Gen);
    begin
        case Rand is
            when 1 => return North;
            when 2 => return South;
            when 3 => return East;
            when others => return West;
        end case;
    end Random_Direction;
    
    function Random_Duration(Min_D, Max_D : Duration) return Duration is
        Range_D : Duration := Max_D - Min_D;
        Rand_Factor : Float := Float(Random_Int.Random(Gen)) / 1000.0;
    begin
        return Min_D + Duration(Rand_Factor * Float(Range_D));
    end Random_Duration;
    
    function Try_Move_Wild_Settler(From_X, From_Y : Integer; Settler_Id : Integer) return Boolean is
        -- Try to move wild settler to adjacent empty field
        Directions : array (1..4) of Direction := (North, South, East, West);
        Current_Pos : Position := (From_X, From_Y);
        Success : Boolean := False;
    begin
        for Dir of Directions loop
            declare
                Next_Pos : Position := Get_Next_Position(Current_Pos, Dir);
                Move_Success : Boolean;
            begin
                Fields(Next_Pos.X, Next_Pos.Y).Request_Wild_Entry(Settler_Id, Move_Success);
                if Move_Success then
                    -- Successfully moved wild settler
                    Fields(From_X, From_Y).Force_Wild_Move(Success);
                    if Success then
                        -- Record the move
                        Time_Manager.Increment_Time;
                        Printer.Add_Move((Time_Manager.Get_Time, Settler_Id, Next_Pos.X, Next_Pos.Y,
                                        Character'Val(Character'Pos('0') + (Settler_Id mod 10))));
                        return True;
                    else
                        -- Rollback if original field wasn't actually wild settler
                        Fields(Next_Pos.X, Next_Pos.Y).Wild_Leave_Field;
                    end if;
                end if;
            end;
        end loop;
        return False;
    end Try_Move_Wild_Settler;

    -- Field management tasks - one per field
    task type Field_Task is
        entry Initialize(Field_X, Field_Y : Integer);
    end Field_Task;
    
    task body Field_Task is
        My_X, My_Y : Integer;
        Wild_Settler_Id : Integer := 0;
        Wild_Active : Boolean := False;
        Last_Spawn_Check : Time := Clock;
        Wild_Lifetime_Start : Time;
        Wild_Lifetime_Duration : Duration;
    begin
        accept Initialize(Field_X, Field_Y : Integer) do
            My_X := Field_X;
            My_Y := Field_Y;
        end Initialize;
        
        loop
            -- Check if wild settler lifetime expired
            if Wild_Active and then Clock - Wild_Lifetime_Start > Wild_Lifetime_Duration then
                Fields(My_X, My_Y).Wild_Leave_Field;
                Wild_Active := False;
                -- Record disappearance
                Time_Manager.Increment_Time;
                Printer.Add_Move((Time_Manager.Get_Time, Wild_Settler_Id, 
                                Board_Width, Board_Height, '.'));
            end if;
            
            -- Check for wild settler spawning
            if not Wild_Active and Fields(My_X, My_Y).Is_Empty then
                if Clock - Last_Spawn_Check > 0.1 then -- 100ms
                    Last_Spawn_Check := Clock;
                    if Float(Random_Int.Random(Gen)) / 1000.0 < Wild_Settler_Spawn_Probability then
                        -- Spawn wild settler
                        Wild_Settler_Id := 1000 + My_X * Board_Height + My_Y;
                        declare
                            Success : Boolean;
                        begin
                            Fields(My_X, My_Y).Request_Wild_Entry(Wild_Settler_Id, Success);
                            if Success then
                                Wild_Active := True;
                                Wild_Lifetime_Start := Clock;
                                Wild_Lifetime_Duration := Random_Duration(Wild_Settler_Min_Lifetime, 
                                                                        Wild_Settler_Max_Lifetime);
                                -- Record wild settler appearance
                                Time_Manager.Increment_Time;
                                Printer.Add_Move((Time_Manager.Get_Time, Wild_Settler_Id, My_X, My_Y, 
                                                Character'Val(Character'Pos('0') + (Wild_Settler_Id mod 10))));
                            end if;
                        end;
                    end if;
                end if;
            end if;
            
            delay 0.01; -- Small delay to prevent busy waiting
        end loop;
    end Field_Task;

    -- Traveler task
    task type Traveler_Task is
        entry Start(Id : Integer);
    end Traveler_Task;
    
    task body Traveler_Task is
        My_Id : Integer;
        Current_Pos : Position;
        Steps_To_Go : Integer;
        Moves : Vector;
        Success : Boolean;
    begin
        accept Start(Id : Integer) do
            My_Id := Id;
        end Start;
        
        -- Initialize random position
        Current_Pos.X := Random_Int.Random(Gen) mod Board_Width;
        Current_Pos.Y := Random_Int.Random(Gen) mod Board_Height;
        Steps_To_Go := Min_Steps + (Random_Int.Random(Gen) mod (Max_Steps - Min_Steps + 1));
        
        -- Try to occupy initial position
        loop
            Fields(Current_Pos.X, Current_Pos.Y).Request_Entry(My_Id, Success);
            if Success then
                exit;
            end if;
            
            -- Check if there's a wild settler that can be moved
            if Fields(Current_Pos.X, Current_Pos.Y).Get_Content = Wild_Settler then
                declare
                    Settler_Id : Integer := Fields(Current_Pos.X, Current_Pos.Y).Get_Occupant_Id;
                begin
                    if Try_Move_Wild_Settler(Current_Pos.X, Current_Pos.Y, Settler_Id) then
                        -- Try to occupy the field again
                        Fields(Current_Pos.X, Current_Pos.Y).Request_Entry(My_Id, Success);
                        if Success then
                            exit;
                        end if;
                    end if;
                end;
            end if;
            
            delay 0.01;
            -- Try different position if current is occupied
            Current_Pos.X := Random_Int.Random(Gen) mod Board_Width;
            Current_Pos.Y := Random_Int.Random(Gen) mod Board_Height;
        end loop;
        
        -- Record initial position
        Time_Manager.Increment_Time;
        Append(Moves, (Time_Manager.Get_Time, My_Id, Current_Pos.X, Current_Pos.Y, 
                      Character'Val(Character'Pos('A') + (My_Id mod 26))));
        
        -- Main movement loop
        for Step in 1 .. Steps_To_Go loop
            delay Random_Duration(Min_Delay, Max_Delay);
            
            declare
                Dir : Direction := Random_Direction;
                Next_Pos : Position := Get_Next_Position(Current_Pos, Dir);
                Move_Success : Boolean := False;
                Attempts : Integer := 0;
            begin
                -- Try to move, with limited attempts
                while not Move_Success and Attempts < 10 loop
                    Fields(Next_Pos.X, Next_Pos.Y).Request_Entry(My_Id, Move_Success);
                    if Move_Success then
                        -- Successfully moved
                        Fields(Current_Pos.X, Current_Pos.Y).Leave_Field;
                        Current_Pos := Next_Pos;
                        Time_Manager.Increment_Time;
                        Append(Moves, (Time_Manager.Get_Time, My_Id, Current_Pos.X, Current_Pos.Y,
                                      Character'Val(Character'Pos('A') + (My_Id mod 26))));
                    else
                        -- Check if there's a wild settler that can be moved
                        if Fields(Next_Pos.X, Next_Pos.Y).Get_Content = Wild_Settler then
                            declare
                                Settler_Id : Integer := Fields(Next_Pos.X, Next_Pos.Y).Get_Occupant_Id;
                            begin
                                if Try_Move_Wild_Settler(Next_Pos.X, Next_Pos.Y, Settler_Id) then
                                    -- Try to move again
                                    Fields(Next_Pos.X, Next_Pos.Y).Request_Entry(My_Id, Move_Success);
                                    if Move_Success then
                                        Fields(Current_Pos.X, Current_Pos.Y).Leave_Field;
                                        Current_Pos := Next_Pos;
                                        Time_Manager.Increment_Time;
                                        Append(Moves, (Time_Manager.Get_Time, My_Id, Current_Pos.X, Current_Pos.Y,
                                                      Character'Val(Character'Pos('A') + (My_Id mod 26))));
                                    end if;
                                end if;
                            end;
                        end if;
                        
                        if not Move_Success then
                            -- Try different direction
                            Dir := Random_Direction;
                            Next_Pos := Get_Next_Position(Current_Pos, Dir);
                            Attempts := Attempts + 1;
                            delay 0.001;
                        end if;
                    end if;
                end loop;
            end;
        end loop;
        
        -- Leave final position and record final state
        Fields(Current_Pos.X, Current_Pos.Y).Leave_Field;
        Append(Moves, (Time_Manager.Get_Time, My_Id, Board_Width, Board_Height, '.'));
        
        -- Send movement history to printer
        for Move of Moves loop
            Printer.Add_Move(Move);
        end loop;
    end Traveler_Task;

    -- Create field tasks
    type Field_Task_Array is array (0 .. Board_Width - 1, 0 .. Board_Height - 1) of Field_Task;
    Field_Tasks : Field_Task_Array;
    
    -- Create traveler tasks
    type Traveler_Array is array (1 .. Nr_Of_Travelers) of Traveler_Task;
    Travelers : Traveler_Array;

begin
    -- Initialize random generators
    Random_Int.Reset(Gen);
    Random_Dir.Reset(Dir_Gen);
    
    -- Print header
    Printer.Print_Header;
    
    -- Initialize field tasks
    for X in 0 .. Board_Width - 1 loop
        for Y in 0 .. Board_Height - 1 loop
            Field_Tasks(X, Y).Initialize(X, Y);
        end loop;
    end loop;
    
    -- Start all travelers
    for I in 1 .. Nr_Of_Travelers loop
        Travelers(I).Start(I);
    end loop;
    
    -- Wait for all travelers to finish (terminate handling)
    -- This is simplified - in real implementation we'd need proper termination
    delay 30.0; -- Give time for simulation to complete
    
    Printer.Finish;
end Extended_Travelers;
