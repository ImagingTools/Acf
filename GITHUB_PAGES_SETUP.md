# GitHub Pages Setup Instructions

This document explains how to enable GitHub Pages for the ACF repository to host the Doxygen documentation and repository statistics.

## Prerequisites

The GitHub Actions workflow has been created in `.github/workflows/stats-pages.yml`. This workflow will automatically generate and deploy both the API documentation and repository statistics when changes are pushed to the `main` or `master` branch.

## What Gets Deployed

The GitHub Pages site includes:

1. **API Documentation**: Generated from source code comments using Doxygen
   - Available at: https://imagingtools.github.io/Acf/
   
2. **Repository Statistics**: Comprehensive code metrics and quality indicators
   - Available at: https://imagingtools.github.io/Acf/stats/
   - Includes: LOC, class counts, component usage, quality score, technical debt indicators
   
See `scripts/README.md` for details about the statistics system.

## Enabling GitHub Pages

To enable GitHub Pages for this repository, follow these steps:

### 1. Navigate to Repository Settings

1. Go to the repository on GitHub: https://github.com/ImagingTools/Acf
2. Click on **Settings** (you need admin access to the repository)

### 2. Configure GitHub Pages

1. In the left sidebar, click on **Pages** under the "Code and automation" section
2. Under **Source**, select **GitHub Actions** from the dropdown menu
3. Click **Save**

### 3. Trigger the Workflow

The workflow will automatically run when:
- Changes are pushed to the `main` or `master` branch
- Manually triggered via the Actions tab

To manually trigger the workflow:
1. Go to the **Actions** tab in the repository
2. Click on **Doxygen Documentation** in the left sidebar
3. Click the **Run workflow** button
4. Select the branch (e.g., `main`) and click **Run workflow**

### 4. Access the Documentation and Statistics

Once the workflow completes successfully:
- The API documentation will be available at: https://imagingtools.github.io/Acf/
- The repository statistics will be available at: https://imagingtools.github.io/Acf/stats/
- You can find the exact URL in the workflow run details under the "deploy" job

## Workflow Details

The workflow (`stats-pages.yml`) performs the following steps:

1. **Generate Statistics**: Analyzes repository with Python script to collect metrics
2. **Generate Doxygen Documentation**: Runs `doxygen Doxyfile` from the `Install/` directory
3. **Combine Content**: Merges statistics page and documentation into single site
4. **Upload Artifact**: Uploads the combined content
5. **Deploy to Pages**: Deploys to GitHub Pages

The workflow runs:
- On push to `main` or `master` branch
- On manual trigger via workflow dispatch
- Weekly on Monday at 00:00 UTC (automated updates)

## Troubleshooting

### Workflow Fails

If the workflow fails:
1. Check the workflow run logs in the **Actions** tab
2. Common issues:
   - Permissions: Ensure the workflow has permission to deploy to Pages
   - Doxyfile errors: Check that the Doxyfile is valid
   - Missing dependencies: Ensure all required tools are installed

### Documentation Not Updating

If the documentation doesn't update after merging changes:
1. Check that the workflow completed successfully in the **Actions** tab
2. Verify that GitHub Pages is configured to use "GitHub Actions" as the source
3. Wait a few minutes for the deployment to propagate

## Modifying the Documentation and Statistics

### API Documentation

The API documentation is generated from:
- **Source code comments**: Doxygen comments in `.h` files
- **Doxyfile**: Configuration in `Install/Doxyfile`
- **Additional pages**: `.dox` files (e.g., `Include/iprm/iprm_mainpage.dox`)

To modify the API documentation:
1. Update source code comments or `.dox` files
2. Commit and push changes to the `main` branch
3. The workflow will automatically regenerate and deploy the documentation

### Repository Statistics

The statistics page is generated from:
- **Python script**: `scripts/generate_stats.py` analyzes the repository
- **HTML template**: `scripts/stats_template.html` displays the results
- **Configuration**: Thresholds and weights can be adjusted in the Python script

To modify statistics:
1. Edit `scripts/generate_stats.py` for new metrics or calculations
2. Edit `scripts/stats_template.html` for display changes
3. See `scripts/README.md` for detailed customization instructions
4. Commit and push changes - statistics update automatically

## Additional Configuration

### Custom Domain

To use a custom domain:
1. Add a `CNAME` file to the repository root with your domain name
2. Configure DNS settings for your domain
3. Update the workflow if needed

### Documentation Settings

To modify Doxygen settings:
1. Edit `Install/Doxyfile`
2. Common settings:
   - `PROJECT_NAME`: Project name shown in documentation
   - `PROJECT_BRIEF`: Short description
   - `OUTPUT_DIRECTORY`: Where to generate documentation
   - `HTML_OUTPUT`: Subdirectory for HTML output

## Support

For issues related to:
- **GitHub Pages**: See [GitHub Pages documentation](https://docs.github.com/en/pages)
- **Doxygen**: See [Doxygen documentation](https://www.doxygen.nl/manual/)
- **Statistics**: See `scripts/README.md` for statistics system documentation
- **Workflow**: Check the workflow file `.github/workflows/stats-pages.yml`
