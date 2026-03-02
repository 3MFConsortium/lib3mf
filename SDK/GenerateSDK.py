#!/usr/bin/env python3
import os
import sys
import shutil
import subprocess
from pathlib import Path
from zipfile import ZipFile, ZIP_DEFLATED

import urllib.request


def failed(msg: str, code: int = 1) -> None:
    print(msg, file=sys.stderr)
    raise SystemExit(code)


def run_git(args: list[str]) -> str:
    try:
        out = subprocess.check_output(["git", *args], stderr=subprocess.STDOUT)
        return out.decode("utf-8", errors="replace").strip()
    except subprocess.CalledProcessError as e:
        failed(f"git failed: {' '.join(args)}\n{e.output.decode('utf-8', errors='replace')}")


def ensure_dir(p: Path, err_msg: str) -> None:
    try:
        p.mkdir(parents=True, exist_ok=True)
    except Exception as e:
        failed(f"{err_msg}: {e}")


def copy_tree(src: Path, dst: Path) -> None:
    if not src.exists():
        failed(f"Missing path: {src}")
    if src.is_dir():
        # Python 3.8+: dirs_exist_ok
        shutil.copytree(src, dst, dirs_exist_ok=True)
    else:
        ensure_dir(dst.parent, f"Error creating folder {dst.parent}")
        shutil.copy2(src, dst)


def download_file(url: str, out_path: Path, timeout_sec: int = 90) -> None:
    ensure_dir(out_path.parent, f"Error creating folder {out_path.parent}")
    req = urllib.request.Request(url, headers={"User-Agent": "lib3mf-sdk-packager/1.0"})
    try:
        with urllib.request.urlopen(req, timeout=timeout_sec) as r, open(out_path, "wb") as f:
            shutil.copyfileobj(r, f)
    except Exception as e:
        failed(f"Error downloading {url} -> {out_path}: {e}")


def obtain_library_cmd(location: str, platform: str, ext: str, subdir: str, artifacts_dir: Path) -> None:
    out_path = artifacts_dir / subdir / f"lib3mf.{ext}"
    if location == "remote":
        buildserver = os.environ.get("BUILDSERVER", "")
        if not buildserver:
            failed("BUILDSERVER env var is not set (required for LOCATION=remote).")

        url = f"{buildserver}/artifact/artifacts/lib3mf_{platform}.{ext}"
        print(f"curl -s --retry 5 -m90 {url} -o {out_path}")
        # emulate: --retry 5, -m 90
        last_err = None
        for _ in range(5):
            try:
                download_file(url, out_path, timeout_sec=90)
                last_err = None
                break
            except SystemExit as e:
                last_err = e
        if last_err is not None:
            raise last_err

        fs = out_path.stat().st_size if out_path.exists() else 0
        if fs < 1000:
            failed(f"Error obtaining latest lib3mf.{ext}. File size too small.")
    else:
        src = Path(location) / f"lib3mf_{platform}.{ext}"
        if not src.exists():
            failed(f"Error obtaining latest lib3mf.{ext}: missing {src}")
        ensure_dir(out_path.parent, f"Error creating {subdir}-folder")
        try:
            shutil.copy2(src, out_path)
        except Exception as e:
            failed(f"Error obtaining latest lib3mf.{ext}: {e}")


def write_version_txt(artifacts_dir: Path) -> None:
    version_txt = artifacts_dir / "version.txt"
    print(f"VERSIONTXT = {version_txt}")

    build_number = os.environ.get("BUILD_NUMBER", "")
    git_branch_env = os.environ.get("GIT_BRANCH", "")

    # bash logic: if [ -n "${GIT_BRANCH#*/}" ] then use it else use git branch
    # Equivalent: if splitting by "/" yields something non-empty after the first "/"
    git_branch_trimmed = ""
    if "/" in git_branch_env:
        git_branch_trimmed = git_branch_env.split("/", 1)[1]
    else:
        git_branch_trimmed = ""

    if git_branch_trimmed:
        git_branch = git_branch_trimmed
    else:
        git_branch = run_git(["rev-parse", "--abbrev-ref", "HEAD"])

    git_rev = run_git(["rev-parse", "HEAD"])

    try:
        with open(version_txt, "w", encoding="utf-8") as f:
            f.write("[Lib3MF_SDK]\n")
            f.write(f"BuildNumber = {build_number}\n")
            f.write(f"GITBranch = {git_branch}\n")
            f.write(f"GITRevision = {git_rev}\n")
    except Exception as e:
        failed(f"Error writing {version_txt}: {e}")


def zip_dir_contents(src_dir: Path, zip_path: Path) -> None:
    # Matches: zip -r OUTFILE SDKARTIFACTS/*
    try:
        if zip_path.exists():
            zip_path.unlink()
        with ZipFile(zip_path, "w", compression=ZIP_DEFLATED) as z:
            for p in sorted(src_dir.rglob("*")):
                if p.is_file():
                    z.write(p, p.relative_to(src_dir))
    except Exception as e:
        failed(f"Error zipping SDK: {e}")


def main(argv: list[str]) -> int:
    sdk_artifacts = Path("lib3mf_sdk")
    location = argv[1] if len(argv) > 1 and argv[1] else "remote"
    outfile = Path(argv[2]) if len(argv) > 2 and argv[2] else Path("lib3mf_sdk.zip")

    print(f"Clean artifacts-folder {sdk_artifacts}")
    if sdk_artifacts.exists():
        shutil.rmtree(sdk_artifacts)
    ensure_dir(sdk_artifacts, "Error creating artifacts-folder")

    print("Copy License, Readme, Examples and Bindings")

    # cat ../LICENSE > License.txt  &  cat Credits.txt >> License.txt
    license_src = Path("../LICENSE")
    credits_src = Path("Credits.txt")
    license_dst = sdk_artifacts / "License.txt"

    if not license_src.exists():
        failed(f"Missing {license_src}")
    if not credits_src.exists():
        failed(f"Missing {credits_src}")

    try:
        license_dst.write_bytes(license_src.read_bytes() + credits_src.read_bytes())
    except Exception as e:
        failed(f"Error creating {license_dst}: {e}")

    copy_tree(Path("Readme.md"), sdk_artifacts / "Readme.md")
    copy_tree(Path("Examples"), sdk_artifacts / "Examples")
    copy_tree(Path("../Autogenerated/Bindings"), sdk_artifacts / "Bindings")

    print("Obtain binaries")
    ensure_dir(sdk_artifacts / "Bin", "Error creating Bin-folder")
    ensure_dir(sdk_artifacts / "Lib", "Error creating Lib-folder")

    obtain_library_cmd(location, "linux64", "so", "Bin", sdk_artifacts)
    obtain_library_cmd(location, "win64", "dll", "Bin", sdk_artifacts)
    obtain_library_cmd(location, "darwin", "dylib", "Bin", sdk_artifacts)
    obtain_library_cmd(location, "win64", "lib", "Lib", sdk_artifacts)

    print("Generate version.txt")
    write_version_txt(sdk_artifacts)

    print("Zip SDK artifacts")
    zip_dir_contents(sdk_artifacts, outfile)

    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv))