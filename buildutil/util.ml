open Printf
open Unix

let init args =
    if Array.length args > 1 then (
        print_string "Error: init takes no arguments\n";
        exit 1
    );
    
    if Sys.file_exist ".watch" then (
        print_string "File .watch already exists.\nContinue with init? Y/n"
        let line = readline in;
        if String.get line 0 == 'n' then (
            exit 1
        );
    );
;;

let () =
    print_string "hello\n"
    let () =
        match Sys.argv.(1) with
        | "init" -> init (Array.sub Sys.argv 1 (Array.length Sys.argv - 1))
        | "check" -> print_string "check\n"
        | _ -> print_string "unknown\n"

