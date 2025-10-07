import React, { useState } from "react";
import "./App.css";

function App() {
  const [selectedFile, setSelectedFile] = useState(null);
  const [enhancedImage, setEnhancedImage] = useState(null);
  const [loading, setLoading] = useState(false);
  const [error, setError] = useState(null);

  const handleFileChange = (e) => {
    const file = e.target.files[0];
    if (file) {
      setSelectedFile(file);
      setEnhancedImage(null);
      setError(null);
    }
  };

  const handleEnhance = async () => {
    if (!selectedFile) return;

    setLoading(true);
    setError(null);
    setEnhancedImage(null);

    const formData = new FormData();
    formData.append("file", selectedFile);

    try {
      const response = await fetch("http://localhost:8000/enhance", {
        method: "POST",
        body: formData,
      });

      if (!response.ok) {
        let errorMessage = `Server error: ${response.status}`;
        try {
          const errorData = await response.json();
          errorMessage = errorData.error || errorMessage;
        } catch {
          errorMessage = response.statusText || errorMessage;
        }
        throw new Error(errorMessage);
      }

      const blob = await response.blob();
      
      if (blob.size === 0) {
        throw new Error('Received empty image');
      }
      
      const url = URL.createObjectURL(blob);
      setEnhancedImage(url);
      
    } catch (err) {
      console.error("Enhancement error:", err);
      setError(err.message);
    } finally {
      setLoading(false);
    }
  };

  const clearError = () => {
    setError(null);
  };

  return (
    <div className="App">
      <div className="app-header">
        <h1>Face Enhancer</h1>
        <p>Upload a photo to enhance faces with AI-powered technology</p>
      </div>

      <div className="upload-container fade-in">
        <div className="file-input-wrapper">
          <input
            type="file"
            accept="image/*"
            onChange={handleFileChange}
            className="file-input"
            id="fileInput"
            disabled={loading}
          />
          <label htmlFor="fileInput" className="file-input-label">
            Choose Image
          </label>
        </div>

        {selectedFile && (
          <div className="file-name">
            <div className="file-info">
              Selected: <strong>{selectedFile.name}</strong> 
              ({(selectedFile.size / 1024).toFixed(1)} KB)
            </div>
          </div>
        )}

        <br />

        <button
          onClick={handleEnhance}
          disabled={loading || !selectedFile}
          className="enhance-button"
        >
          {loading ? "Processing..." : "Enhance Image"}
        </button>

        {loading && (
          <div className="loading-container fade-in">
            <div className="spinner"></div>
            <div className="loading-text">
              Enhancing your image... This may take a few moments
            </div>
          </div>
        )}
      </div>

      {error && (
        <div className="error-message fade-in">
          <h3>Error</h3>
          <p>{error}</p>
          <button onClick={clearError} className="dismiss-button">
            Dismiss
          </button>
        </div>
      )}

      <div className="images-container">
        {selectedFile && (
          <div className="image-card fade-in">
            <h3>Original Image</h3>
            <img
              src={URL.createObjectURL(selectedFile)}
              alt="original"
              className="image-preview"
            />
          </div>
        )}

        {enhancedImage && (
          <div className="image-card fade-in">
            <h3>Enhanced Image</h3>
            <img
              src={enhancedImage}
              alt="enhanced"
              className="image-preview"
            />
            <a
              href={enhancedImage}
              download="enhanced_image.jpg"
              className="download-button"
            >
              Download Enhanced Image
            </a>
          </div>
        )}
      </div>
    </div>
  );
}

export default App;