use reqwest::blocking::get;
use serde::{Deserialize, Serialize};

#[derive(Serialize, Deserialize, Debug)]
#[allow(non_snake_case)]
struct Item {
    Name: String,
    Homepage: Option<String>,
}

#[derive(Serialize, Deserialize, Debug)]
#[allow(non_snake_case)]
struct Obj {
    Baseline: String,
    Size: usize,
    Source: Vec<Item>,
}

fn main() -> Result<(), Box<dyn std::error::Error>> {
    let resp: Obj = get("https://vcpkg.io/output.json")?.json()?;
    let mut filtered = Vec::new();
    for item in resp.Source.iter() {
        if let Some(val) = item.Homepage.clone() {
            if val.contains("github") {
                filtered.push(item);
            }
        }
    }

    if let Ok(string) = serde_json::to_string(&filtered) {
        match std::fs::write("sample.json", string) {
            Ok(_) => {
                println!("File Succesfully written");
            }
            Err(_) => {
                println!("Could not write file for some reason")
            }
        }
    }

    Ok(())
}
