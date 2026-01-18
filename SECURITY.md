# Security Policy

## Reporting Security Vulnerabilities

The ACF (Application Component Framework) team takes security seriously. We appreciate your efforts to responsibly disclose your findings.

### How to Report a Security Vulnerability

If you discover a security vulnerability in ACF, please report it by:

1. **Email**: Send details to the project maintainers via GitHub
2. **Private Security Advisory**: Use GitHub's private vulnerability reporting feature

**Please do not** report security vulnerabilities through public GitHub issues.

### What to Include in Your Report

To help us triage and fix the issue quickly, please include:

- Description of the vulnerability
- Steps to reproduce the issue
- Potential impact of the vulnerability
- Any suggested fixes or mitigations
- Your contact information for follow-up

### Response Timeline

- **Initial Response**: We aim to acknowledge receipt within 72 hours
- **Status Updates**: We will provide updates on the progress every 7 days
- **Resolution**: We aim to release a fix within 90 days, depending on complexity

## Security Update Policy

### Supported Versions

We provide security updates for the following versions:

| Version | Supported          |
| ------- | ------------------ |
| Latest release | :white_check_mark: |
| Previous release | :white_check_mark: |
| Older versions | :x: |

We recommend always using the latest version to ensure you have all security fixes.

### Security Advisories

Security advisories will be published through:

- GitHub Security Advisories
- Release notes
- Project documentation

## Known Security Considerations

### Build Environment Security

ACF is built using CMake, QMake, and Qbs build systems. Users should:

- Verify the integrity of downloaded releases
- Use trusted sources for dependencies (Qt framework)
- Review build configurations before compilation

### Dependency Management

ACF depends on:

- Qt Framework (minimum version 5.15, also compatible with Qt 6.x)
- Standard C++ libraries

Users are responsible for:

- Keeping Qt and system libraries up to date
- Monitoring security advisories for dependencies
- Applying security patches promptly

### Safe Usage Guidelines

When using ACF in your applications:

- Validate all external inputs before processing
- Follow secure coding practices when handling user data
- Implement appropriate access controls in applications
- Use secure communication channels for sensitive data

## Vulnerability Disclosure Policy

We follow responsible disclosure practices:

1. **Private Disclosure**: Vulnerabilities are handled privately until fixed
2. **Coordinated Release**: Security fixes are released with advisories
3. **Credit**: We acknowledge security researchers (unless they prefer anonymity)
4. **CVE Assignment**: Critical vulnerabilities receive CVE identifiers when applicable

## Security Best Practices for Contributors

If you're contributing to ACF:

- Follow secure coding guidelines
- Review code for potential security issues
- Test security-sensitive changes thoroughly
- Document security implications of new features
- Never commit secrets, credentials, or sensitive data

## Compliance

### EU Cyber Resilience Act (CRA)

This project aims to comply with the EU Cyber Resilience Act requirements:

- **Vulnerability Management**: We maintain a process for identifying, documenting, and remediating vulnerabilities
- **Security Updates**: Security patches are provided during the support period
- **Transparency**: Security information is publicly available
- **SBOM**: Software Bill of Materials can be generated for compliance

### License

ACF is licensed under the GNU Lesser General Public License v2.1 (LGPL-2.1). See the [LICENSE](LICENSE) file for details.

## Contact

For security-related questions or concerns:

- Review existing [security advisories](https://github.com/ImagingTools/Acf/security/advisories)
- Contact the maintainers through GitHub
- Check the [documentation](https://imagingtools.github.io/Acf/) for security guidelines

## Updates to This Policy

This security policy may be updated periodically. Check back regularly for the latest information.

**Last Updated**: January 2026
