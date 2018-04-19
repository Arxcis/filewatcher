import sys
import glob
import os
import time
from watchdog.observers import Observer
from watchdog.events import PatternMatchingEventHandler


def post(event_type, collection, tag, extension, filepath):
    print(event_type, collection, tag, extension, filepath)
    sys.stdout.flush()

class CollectionHandler(PatternMatchingEventHandler):

    def __init__(self, collection,
                       patterns=None, 
                       ignore_patterns=None, 
                       ignore_directories=False, 
                       case_sensitive=True):
        super().__init__(patterns, 
                         ignore_patterns,
                         ignore_directories, 
                         case_sensitive)

        self.collection = collection

    def process(self, event):
        """
        event.event_type 
            'modified' | 'created' | 'moved' | 'deleted'
        event.is_collection
            True | False
        event.src_path
            path/to/observed/file
        """
        fname = os.path.basename(event.src_path)
        
        post(event_type = event.event_type,
             collection = self.collection,
             tag        = fname.split(".")[0],
             extension  = fname.split(".")[1],
             filepath   = event.src_path)

    def on_modified(self, event):
        self.process(event)

    def on_created(self, event):
        self.process(event)

    def on_deleted(self, event):
        self.process(event)

    def on_moved(self, event):
        self.process(event)



def discover_collection_files(collection, extension_match):

    for ext in extension_match:
        filenames = glob.glob("./assets/"+collection+"/"+ext)

        for fname in filenames:

            fname = os.path.basename(fname)

            post(event_type = "discovered",
                 collection = collection,
                 tag        = fname.split(".")[0],
                 extension  = fname.split(".")[1],
                 filepath   = "./assets/"+collection+"/"+fname)


if __name__ == '__main__':

    args = sys.argv[1:]
    observer = Observer()

    collections = ["textures", "shaders", "materials", "models", "scenes"]
    extensions  = [["*.jpg", "*.png", "*.gif"],  ["*.glsl"], ["*.yml"],  ["*.yml"],  ["*.yml"]]

    for c, e in zip(collections, extensions):
        discover_collection_files(c, e)
        observer.schedule(CollectionHandler(c,  e),  "./assets/"+c+"/")


    observer.start()

    try:
        while True:
            time.sleep(1)
    except KeyboardInterrupt:
        print("KEYBOARD INTERRUPT")
        sys.stdout.flush()
        observer.stop()

    observer.join()

