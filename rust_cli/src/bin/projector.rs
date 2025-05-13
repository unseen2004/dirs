use clap::Parser;
use rust::{Opts, Config}; 

fn main() -> anyhow::Result<()> {
    let cfg: Config = Opts::parse().try_into()?;
    println!("{cfg:#?}");
    Ok(())
}

