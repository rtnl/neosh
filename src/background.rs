use uuid::Uuid;

#[derive(Debug, Clone)]
pub struct BackgroundProcess {
    id: Uuid,
    child_id: u32,
}

impl BackgroundProcess {
    pub fn new(child_id: u32) -> Self {
        Self {
            id: Uuid::new_v4(),
            child_id,
        }
    }

    pub fn get_id(&self) -> Uuid {
        self.id
    }

    pub fn get_child_id(&self) -> u32 {
        self.child_id
    }
}
