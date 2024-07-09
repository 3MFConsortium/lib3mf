import argparse
import urllib.request
import json

def get_sdk_url(index):
    url = "https://api.github.com/repos/3MFConsortium/lib3mf/releases"
    
    try:
        with urllib.request.urlopen(url) as response:
            data = response.read().decode('utf-8')
        
        releases = json.loads(data)
        selected_release = releases[index]  # Select the release by index
        for asset in selected_release['assets']:
            asset_name = str(asset['name']) 
            if asset_name.startswith("lib3mf_sdk") and asset_name.endswith(".zip"):
                return asset['browser_download_url']
        return None
    except Exception as e:
        print(f"Error fetching the SDK URL: {e}")
        return None

def main():
    parser = argparse.ArgumentParser(description="Fetch a specific release URL for the 3MFConsortium/lib3mf repository.")
    parser.add_argument('index', type=int, help='Index of the release (0 for latest, 1 for second latest, etc.)')
    parser.add_argument('--get-url', action='store_true', help='Get the specified release URL')

    args = parser.parse_args()

    if args.get_url:
        url = get_sdk_url(args.index)
        if url:
            print(url)
        else:
            print("FAIL")

if __name__ == "__main__":
    main()
