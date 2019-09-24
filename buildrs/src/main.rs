use std::env;
use std::fs::File;
use std::io::BufReader;
use std::io::BufRead;

use preprocess::preprocess;

struct Config {
    runfile: String,
    args: Vec<String>
}

fn main() {
    let config = parse_commands();
    let commands: Vec<String> = preprocess(file, &args);
}

fn parse_arg_file(name: &str) -> Vec<String> {
    let file = File::open(name).expect("Can't open arg file");
    let file = BufReader::new(&file);
    let mut ret: Vec<String> = Vec::new();
    for line in file.lines() {
        ret.push(line.unwrap())
    }
    ret
}

fn parse_commands() -> Config {
    let args: Vec<String> = env::args().collect();
    if args.len() == 1 {
        return Config {
            runfile: String::from("Runfile"),
            args: Vec::new()
        }
    }
    
    if args.len() == 2 {
        return Config {
            runfile: String::from("Runfile"),
            args: parse_arg_file(&args[1])
        }
    }
    
    if args.len() == 3 {
        return Config {
            runfile: args[2].clone(),
            args: parse_arg_file(&args[1])
        }
    }

    Config {
        runfile: args[1].clone(),
        args: args[2..].to_vec()
    }
}