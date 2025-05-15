use std::{collections::HashMap, path::PathBuf};
use serde::{Serialize, Deserialize};
use crate::config::Config;

#[derive(Debug, Serialize, Deserialize)]
struct Data {
    pub projector: HashMap<PathBuf, HashMap<String, String>>
}

pub struct Projector {
    config: Config,
    data: Data,
}

fn default_data() -> Data {
    return Data {
        projector: HashMap::new(),  // Fixed: semicolon to comma
    };
}

impl Projector {
     pub fn get_value_all(&self) -> HashMap<String, String> {
        let mut curr = Some(self.config.pwd.as_path());
        let mut paths = vec![];
        while let Some(p) = curr {
            paths.push(p);
            curr = p.parent()
        }

        let mut out = HashMap::new();
        for path in paths.into_iter().rev() {
            if let Some(map) = self.data.projector.get(path) {
                // Fixed: Clone keys and values for correct return type
                for (k, v) in map.iter() {
                    out.insert(k.clone(), v.clone());
                }
            }
        }

        return out;
    }

    pub fn get_value(&self, key: &str) -> Option<&String> {
        let mut curr = Some(self.config.pwd.as_path());
        let mut out = None;

        while let Some(p) = curr {
            // Fixed: Type annotation for get() method
            if let Some(dir) = self.data.projector.get(&p.to_path_buf()) {
                if let Some(value) = dir.get(key) {
                    out = Some(value);
                    break;
                }
            }
            curr = p.parent();
        }
        return out;
    }

    pub fn set_value(&mut self, key: String, value: String) {
        self.data.projector 
            .entry(self.config.pwd.clone())
            .or_default() 
            .insert(key, value);
    }

    pub fn remove_value(&mut self, key: &str) {
        self.data.projector 
            .entry(self.config.pwd.clone())
            .or_default() 
            .remove(key);  // Fixed: use remove() instead of insert()
    }

    pub fn from_config(config: Config) -> Self {
        if std::fs::metadata(&config.config).is_ok() {
            let contents = std::fs::read_to_string(&config.config);
            let contents = contents.unwrap_or(
                String::from("{\"projector\":{}}"));
            let data = serde_json::from_str(&contents);
            let data = data.unwrap_or(default_data());

            return Projector {
                config,
                data,
            }
        }

        return Projector {
            config,
            data: default_data(),
        }
    }
}

#[cfg(test)]
mod test {
    use super::*;
    use crate::config::{Config, Operation};
    use std::{path::PathBuf, collections::HashMap};

    // Fixed: Replace hashmap! macro with HashMap construction function
    fn get_data() -> HashMap<PathBuf, HashMap<String, String>> {
        let mut data = HashMap::new();
        
        let mut root_map = HashMap::new();
        root_map.insert("foo".to_string(), "bar1".to_string());
        root_map.insert("fem".to_string(), "is_great".to_string());
        data.insert(PathBuf::from("/"), root_map);
        
        let mut foo_map = HashMap::new();
        foo_map.insert("foo".to_string(), "bar2".to_string());
        data.insert(PathBuf::from("/foo"), foo_map);
        
        let mut bar_map = HashMap::new();
        bar_map.insert("foo".to_string(), "bar3".to_string());
        data.insert(PathBuf::from("/foo/bar"), bar_map);
        
        data
    }

    fn get_projector(pwd: PathBuf) -> Projector {
        return Projector {
            config: Config {
                pwd,
                config: PathBuf::from(""),
                operation: crate::config::Operation::Print(None),
            },
            data: Data {
                projector: get_data()
            }
        }
    }

    #[test]
    fn get_value() {
        let proj = get_projector(PathBuf::from("/foo/bar"));
        assert_eq!(proj.get_value("foo"), Some(&String::from("bar3")));
        assert_eq!(proj.get_value("fem"), Some(&String::from("is_great")));
    }

     #[test]
    fn set_value() {
        let mut proj = get_projector(PathBuf::from("/foo/bar"));
        proj.set_value(String::from("foo"), String::from("bar4"));
        proj.set_value(String::from("fem"), String::from("bar5"));
        
        assert_eq!(proj.get_value("foo"), Some(&String::from("bar4")));
        assert_eq!(proj.get_value("fem"), Some(&String::from("bar5")));
    }
}
