# GitHub Pages Setup Instructions

This document explains how to enable GitHub Pages for the ACF repository to host the Doxygen documentation.

## Prerequisites

The GitHub Actions workflow has been created in `.github/workflows/doxygen-pages.yml`. This workflow will automatically generate and deploy the documentation when changes are pushed to the `main` or `master` branch.

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

### 4. Access the Documentation

Once the workflow completes successfully:
- The documentation will be available at: https://imagingtools.github.io/Acf/
- You can find the exact URL in the workflow run details under the "deploy" job

## Workflow Details

The workflow performs the following steps:

1. **Checkout**: Checks out the repository code
2. **Install Doxygen**: Installs Doxygen and Graphviz on Ubuntu
3. **Generate Documentation**: Runs `doxygen Doxyfile` from the `Install/` directory
4. **Upload Artifact**: Uploads the generated HTML documentation
5. **Deploy to Pages**: Deploys the documentation to GitHub Pages

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

## Modifying the Documentation

The documentation is generated from:
- **Source code comments**: Doxygen comments in `.h` files
- **Doxyfile**: Configuration in `Install/Doxyfile`
- **Additional pages**: `.dox` files (e.g., `Include/iprm/iprm_mainpage.dox`)

To modify the documentation:
1. Update source code comments or `.dox` files
2. Commit and push changes to the `main` branch
3. The workflow will automatically regenerate and deploy the documentation

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
- **Workflow**: Check the workflow file `.github/workflows/doxygen-pages.yml`
