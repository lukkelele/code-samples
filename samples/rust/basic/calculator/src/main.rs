use std::io::{self, Write};
use std::process::ExitCode;

fn eval(a: f64, op: &str, b: f64) -> Option<f64> {
    match op {
        "+" => Some(a + b),
        "-" => Some(a - b),
        "x" | "*" => Some(a * b),
        "/" if b != 0.0 => Some(a / b),
        _ => None,
    }
}

fn evaluate_line(line: &str) -> Result<f64, String> {
    let tokens: Vec<&str> = line.split_whitespace().collect();
    if tokens.len() != 3 {
        return Err(String::from("expected: <a> <op> <b>"));
    }

    let a: f64 = tokens[0]
        .parse()
        .map_err(|_| format!("invalid number: {}", tokens[0]))?;
    let b: f64 = tokens[2]
        .parse()
        .map_err(|_| format!("invalid number: {}", tokens[2]))?;

    eval(a, tokens[1], b).ok_or_else(|| format!("invalid operation: {line}"))
}

fn main() -> ExitCode {
    let stdin = io::stdin();
    let mut stdout = io::stdout();
    let mut line = String::new();

    println!("");
    println!("calculator");
    println!(" q/quit/exit: exit application");
    println!("");
    println!(" enter: <a> <op> <b>");
    println!("  op: + - x /");

    loop {
        print!("> ");
        if stdout.flush().is_err() {
            return ExitCode::FAILURE;
        }

        line.clear();
        match stdin.read_line(&mut line) {
            Ok(0) => break,
            Ok(_) => {}
            Err(error) => {
                eprintln!("read error: {error}");
                return ExitCode::FAILURE;
            }
        }

        let trimmed = line.trim();
        if trimmed.is_empty() {
            continue;
        }
        if matches!(trimmed, "quit" | "exit" | "q") {
            break;
        }

        match evaluate_line(trimmed) {
            Ok(result) => println!("{result}"),
            Err(message) => eprintln!("{message}"),
        }
    }

    ExitCode::SUCCESS
}
