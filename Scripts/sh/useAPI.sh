
#!/bin/bash

# Function to fetch and display cat image from The Cat API
fetch_cat_image() {
  echo "Fetching random cat image..."
  
  # Get random cat image URL from The Cat API (JSON response)
  cat_url=$(curl -s https://api.thecatapi.com/v1/images/search | jq -r '.[0].url')

  # Download the image to a temporary file
  tmp_file=$(mktemp /tmp/cat_image.XXXXXX.jpg)
  curl -s -o "$tmp_file" "$cat_url"

  # Check if catimg is available and use it
  if command -v catimg &> /dev/null; then
    # Use catimg to display the image (better suited for terminal)
    catimg "$tmp_file"
  elif command -v img2txt &> /dev/null; then
    # Use img2txt to display the image, ensuring proper formatting
    img2txt -W 80 -H 40 -f ansi "$tmp_file"
  else
    echo "Neither catimg nor img2txt is installed. Unable to display image."
  fi

  # Clean up temporary image file
  rm "$tmp_file"
}

# Function to fetch and display a random Chuck Norris joke
fetch_chuck_norris_joke() {
  echo "Fetching Chuck Norris joke..."

  # Get random Chuck Norris joke from the API (JSON response)
  joke=$(curl -s https://api.chucknorris.io/jokes/random | jq -r '.value')

  # Display the joke
  echo -e "\nChuck Norris Joke: $joke"
}

# Main script execution
fetch_cat_image
fetch_chuck_norris_joke

