from fastapi import FastAPI, File, UploadFile
from fastapi.responses import FileResponse
from fastapi.middleware.cors import CORSMiddleware
import os
import subprocess
import uuid

app = FastAPI()

UPLOAD_FOLDER = "uploads"
OUTPUT_FOLDER = "outputs"
os.makedirs(UPLOAD_FOLDER, exist_ok=True)
os.makedirs(OUTPUT_FOLDER, exist_ok=True)

origins = [
    "http://localhost:5173",
    "http://127.0.0.1:5173",
    "http://localhost:5173",
    "http://127.0.0.1:5173",
]

app.add_middleware(
    CORSMiddleware,
    allow_origins=origins,
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

CXX_EXECUTABLE = r"D:\face_enhancer\backend\face_enhancer.exe"

@app.post("/enhance")
async def enhance_image(file: UploadFile = File(...)):
    input_id = str(uuid.uuid4())
    input_path = os.path.join(UPLOAD_FOLDER, f"{input_id}.jpg")
    output_path = os.path.join(OUTPUT_FOLDER, f"{input_id}_enhanced.jpg")

    print(f"Processing: {input_path} -> {output_path}")

    try:
        with open(input_path, "wb") as f:
            content = await file.read()
            f.write(content)
        print(f"Uploaded file saved: {len(content)} bytes")
    except Exception as e:
        print(f"Error saving file: {e}")
        return {"error": f"Failed to save uploaded file: {str(e)}"}

 
    try:
        print(f"Running C++ executable: {CXX_EXECUTABLE}")
        
        result = subprocess.run(
            [CXX_EXECUTABLE, input_path, output_path],
            check=True,
            capture_output=True,
            text=True,
            timeout=120,
            shell=True
        )
        print("C++ stdout:", result.stdout)
        if result.stderr:
            print("C++ stderr:", result.stderr)
            
    except subprocess.TimeoutExpired:
        print("C++ executable timed out")
        return {"error": "Processing timed out"}
    except subprocess.CalledProcessError as e:
        print("C++ executable failed:", e)
        print("stdout:", e.stdout)
        print("stderr:", e.stderr)
        return {"error": f"Face enhancement failed: {str(e)}"}
    except Exception as e:
        print("Unexpected error:", e)
        return {"error": f"Unexpected error: {str(e)}"}

    if not os.path.exists(output_path):
        print("Output file not found:", output_path)
        return {"error": "Output file not created"}

    print(f"Success Enhanced image created: {output_path}")

    try:
        return FileResponse(
            output_path, 
            media_type="image/jpeg",
            filename="enhanced_image.jpg"
        )
    except Exception as e:
        print(f"Error returning file: {e}")
        return {"error": f"Failed to return enhanced image: {str(e)}"}



