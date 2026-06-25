#!/usr/bin/env python3
#
# Copyright 2026 Kritik Agarwal
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
# Purpose:
# Synchronize the embedded Garlic core in Garlic GUI with the latest
# upstream Garlic source while preserving Garlic GUI-specific files and
# applying downstream patches.
#
# Upstream Repository:
# https://github.com/neocanable/garlic
#
# Operations:
#   1. Fetch the latest upstream Garlic source.
#   2. Synchronize the `shell/` directory.
#   3. Synchronize the `src/` directory into `src/garlic/`.
#   4. Preserve Garlic GUI wrapper files (`garlic_wrapper.c/.h`).
#   5. Apply Garlic GUI-specific patches:
#      - expression_writer.c: Add Garlic GUI decompiler notice.
#      - threadpool.c: Add mutex protection for thread-local allocation.
#
# Run this script whenever updating the embedded Garlic core.

import shutil
import subprocess
from pathlib import Path
import tempfile

ROOT = Path(__file__).resolve().parent.parent

UPSTREAM = "https://github.com/neocanable/garlic.git"

TMP = Path(tempfile.gettempdir()) / "garlic-upstream"

SRC = TMP / "src"
SHELL = TMP / "shell"

DEST_SRC = ROOT / "src" / "garlic"
DEST_SHELL = ROOT / "shell"


def run(cmd):
    subprocess.run(cmd, check=True)


def clone_or_update():
    if TMP.exists():
        print("Updating upstream...")
        run(["git", "-C", str(TMP), "fetch", "--all"])
        run(["git", "-C", str(TMP), "reset", "--hard", "origin/main"])
    else:
        print("Cloning upstream...")
        run(["git", "clone", "--depth", "1", UPSTREAM, str(TMP)])


def backup_wrapper():
    wrappers = {}

    for f in [
        "garlic_wrapper.c",
        "garlic_wrapper.h",
    ]:
        path = DEST_SRC / f
        if path.exists():
            wrappers[f] = path.read_bytes()

    return wrappers


def restore_wrapper(wrappers):
    for name, data in wrappers.items():
        (DEST_SRC / name).write_bytes(data)


def sync_shell():
    if DEST_SHELL.exists():
        shutil.rmtree(DEST_SHELL)

    shutil.copytree(SHELL, DEST_SHELL)


def sync_src():
    wrappers = backup_wrapper()

    for item in SRC.iterdir():
        dst = DEST_SRC / item.name

        if item.name in (
            "garlic_wrapper.c",
            "garlic_wrapper.h",
        ):
            continue

        if dst.exists():
            if dst.is_dir():
                shutil.rmtree(dst)
            else:
                dst.unlink()

        if item.is_dir():
            shutil.copytree(item, dst)
        else:
            shutil.copy2(item, dst)

    restore_wrapper(wrappers)


def patch_expression_writer():
    file = DEST_SRC / "decompiler" / "expression_writer.c"

    text = file.read_text()

    needle = 'fprintf(stream, " * Decompiled by Garlic\\n");'

    insert = '''fprintf(stream, " * Decompiled by Garlic\\n");
    fprintf(stream, " * Using Garlic-GUI Decompiler by AgarwalKritik\\n");'''

    if "Garlic-GUI Decompiler" not in text:
        text = text.replace(needle, insert)

    file.write_text(text)


def patch_threadpool():
    file = DEST_SRC / "libs" / "threadpool" / "threadpool.c"

    text = file.read_text()

    if "pthread_mutex_lock(pool->lock);" in text:
        return

    old = """thread_local_data *tls = x_alloc_in(mpool, sizeof(thread_local_data));"""

    new = """pthread_mutex_lock(pool->lock);
    thread_local_data *tls = x_alloc_in(mpool, sizeof(thread_local_data));
    pthread_mutex_unlock(pool->lock);"""

    text = text.replace(old, new)

    file.write_text(text)


def main():
    clone_or_update()

    print("Syncing shell...")
    sync_shell()

    print("Syncing src...")
    sync_src()

    print("Applying patches...")
    patch_expression_writer()
    patch_threadpool()

    print("Done.")


if __name__ == "__main__":
    main()