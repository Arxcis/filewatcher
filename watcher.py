import sys
import os
import time
import logging
from watchdog.observers import Observer
from watchdog.events import PatternMatchingEventHandler
class MyHandler(PatternMatchingEventHandler):

    base = "assets"

    def __init__(self, directory,
                       patterns=None, 
                       ignore_patterns=None, 
                       ignore_directories=False, 
                       case_sensitive=True):
        super().__init__(patterns, 
                         ignore_patterns,
                         ignore_directories, 
                         case_sensitive)

        self.directory = directory


    def process(self, event):
        """
        event.event_type 
            'modified' | 'created' | 'moved' | 'deleted'
        event.is_directory
            True | False
        event.src_path
            path/to/observed/file
        """
        filename = os.path.basename(event.src_path)
        tag = filename.split(".")[0]
        print(self.directory, filename, tag, event.event_type, event.src_path)
        sys.stdout.flush()

    def on_modified(self, event):
        self.process(event)

    def on_created(self, event):
        self.process(event)


if __name__ == '__main__':
   # print("hello world")
    sys.stdout.flush()

    args = sys.argv[1:]
    observer = Observer()
    observer.schedule(MyHandler("shaders",   ["*.glsl"]), "./assets/shaders/")
    # observer.schedule(MyHandler("textures",  ["*.jpg"]),  "./assets/textures/")
    observer.schedule(MyHandler("materials", ["*.yml"]),  "./assets/materials/")
    observer.schedule(MyHandler("scenes",    ["*.yml"]),  "./assets/scenes/")
    observer.schedule(MyHandler("models",    ["*.yml"]),  "./assets/models/")
    observer.start()

    try:
        while True:
            time.sleep(1)
    except KeyboardInterrupt:
        print("KEYBOARD INTERRUPT")
        sys.stdout.flush()
        observer.stop()

    observer.join()

