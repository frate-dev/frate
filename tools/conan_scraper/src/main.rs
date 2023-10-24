#![allow(unused)]
use std::{
    io::Result,
    sync::{Arc, Mutex},
    thread::{self, JoinHandle},
    time::Duration,
};

use rayon::prelude::*;
use reqwest::blocking::get;
use scraper::*;
use serde::{Deserialize, Serialize};
use serde_json::*;

const URL: &'static str = "https://conan.io/center/recipes";

#[derive(Debug, Serialize, Deserialize, Clone)]
struct Item {
    name: String,
    href: String,
}

#[derive(Debug, Serialize, Deserialize)]
struct ConanData {
    data: Vec<Item>,
}
impl ConanData {
    fn new() -> Self {
        Self { data: Vec::new() }
    }
}

fn main() {
    let mut data = ConanData::new();

    let mut t_array: Arc<Mutex<Vec<Item>>> = Arc::new(Mutex::new(Vec::new()));

    let html = get(URL).unwrap().text().unwrap();
    let doc = Html::parse_document(html.as_str());
    let list_selector = Selector::parse(".list-group .list-group-item").unwrap();

    let mut elements = Vec::new();
    for element in doc.select(&list_selector) {
        elements.push(element.html().clone());
    }
    let mut threads = Vec::new();

    for element in elements {
        let t_array_clone = t_array.clone();
        thread::sleep(Duration::from_millis(51));
        threads.push(thread::spawn(move || {
            let link_selector = Selector::parse("a").unwrap();
            let name_selector = Selector::parse("a h3").unwrap();

            let doc = Html::parse_document(&element.clone());

            let link = doc
                .select(&link_selector.clone())
                .next()
                .unwrap()
                .value()
                .attr("href");

            let name = doc
                .select(&name_selector.clone())
                .next()
                .unwrap()
                .inner_html()
                .replace("<!-- -->/<!-- -->", " ")
                .split(" ")
                .next()
                .unwrap()
                .to_owned();

            let mut item = Item {
                name,
                href: link.unwrap().to_owned(),
            };

            scrape(&mut item);
            {
                let mut lock = t_array_clone.lock().unwrap();
                lock.push(item.clone());
            }
        }));
    }
    for t in threads {
        t.join();
    }

    let lock = t_array.lock().unwrap().clone();
    data.data = lock;
    if let Ok(string) = serde_json::to_string(&data) {
        match std::fs::write("conan_sample.json", string) {
            Ok(_) => {
                println!("File Succesfully written");
            }
            Err(_) => {
                println!("Could not write file for some reason")
            }
        }
    }
}
fn scrape(item: &mut Item) {
    let mut item = item.clone();
    if let Ok(html) = get(format!("https://conan.io{}", item.href)) {
        let html = html.text().unwrap();
        let github_link_selector = Selector::parse("a").unwrap();
        let doc = Html::parse_document(html.as_str());

        for link in doc.select(&github_link_selector) {
            if let Some(l) = link.value().attr("href") {
                if l.contains("github") && !l.contains("conan") {
                    item.href = l.to_owned();
                    println!("{:#?}", &item);
                } else {
                }
            } else {
            }
        }
    } else {
        scrape(&mut item);
    }
}
