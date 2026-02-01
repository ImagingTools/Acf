# Contributing to ACF

Thank you for your interest in contributing to the ACF (Application Component Framework) project!

## How to Contribute

We welcome contributions in the form of:

- Bug reports
- Feature requests
- Code contributions
- Documentation improvements
- Security vulnerability reports (see [SECURITY.md](SECURITY.md))

## Getting Started

1. **Fork the Repository**: Create a fork of the ACF repository
2. **Clone Your Fork**: Clone your fork locally
3. **Create a Branch**: Create a feature branch for your changes
4. **Make Changes**: Implement your changes following our guidelines
5. **Test**: Ensure your changes work correctly
6. **Submit a Pull Request**: Submit a PR with a clear description

## Development Guidelines

### Code Style

- Follow the existing code style in the project
- Use consistent indentation and formatting
- Write clear, self-documenting code
- Add comments for complex logic

### Building and Testing

- Ensure your code builds without errors
- Test your changes on relevant platforms
- Add or update tests as needed
- Run existing tests to ensure no regressions

For build instructions, see the project documentation.

### Documentation

- Update documentation for new features
- Keep documentation in sync with code changes
- Use clear and concise language
- Include examples where helpful

## Security Considerations

### Security Best Practices

When contributing to ACF, please follow these security guidelines:

1. **Secure Coding Practices**
   - Validate all inputs
   - Avoid buffer overflows and memory leaks
   - Use safe C++ practices (RAII, smart pointers)
   - Be cautious with type conversions

2. **Dependency Management**
   - Avoid adding unnecessary dependencies
   - Use well-maintained libraries with active security updates
   - Document new dependencies in the SBOM
   - Check dependencies for known vulnerabilities

3. **Data Handling**
   - Never hardcode credentials or secrets
   - Sanitize user inputs
   - Validate data before serialization/deserialization
   - Use secure random number generation when needed

4. **Error Handling**
   - Handle errors gracefully
   - Avoid exposing sensitive information in error messages
   - Log security-relevant events appropriately
   - Don't ignore security-critical errors

### Reporting Security Issues

**Do NOT report security vulnerabilities through public issues.**

If you discover a security vulnerability:

1. Follow the process in [SECURITY.md](SECURITY.md)
2. Report privately through GitHub Security Advisories
3. Allow time for the issue to be addressed before public disclosure

See [SECURITY.md](SECURITY.md) for our complete security policy.

## Pull Request Process

1. **Update Documentation**: Include documentation updates with code changes
2. **Follow Guidelines**: Adhere to code style and security guidelines
3. **Test Your Changes**: Ensure all tests pass
4. **Update SBOM**: If you add dependencies, update [sbom.json](sbom.json)
5. **Clear Description**: Provide a clear description of your changes
6. **Link Issues**: Reference any related issues

### Pull Request Checklist

- [ ] Code follows project style guidelines
- [ ] Changes have been tested
- [ ] Documentation has been updated
- [ ] No security issues introduced
- [ ] Dependencies documented in SBOM (if applicable)
- [ ] Commit messages are clear and descriptive
- [ ] PR description explains the changes

## Code Review

- All contributions require code review
- Reviews check for functionality, style, and security
- Address reviewer feedback promptly
- Be open to suggestions and improvements

## Licensing

By contributing to ACF, you agree that your contributions will be licensed under the GNU Lesser General Public License v2.1 (LGPL-2.1).

- Ensure you have the right to contribute the code
- Do not include code with incompatible licenses
- Credit third-party code appropriately

## EU Cyber Resilience Act Compliance

ACF aims to comply with the EU Cyber Resilience Act (CRA). When contributing:

- **Security by Design**: Consider security in your design and implementation
- **Vulnerability Management**: Report and help fix security issues
- **Transparency**: Document security-relevant aspects of your changes
- **SBOM**: Update the Software Bill of Materials for new dependencies

See [CRA_COMPLIANCE.md](CRA_COMPLIANCE.md) for more information.

## Dependency Updates

When updating dependencies:

1. **Check for Vulnerabilities**: Ensure the new version fixes security issues
2. **Test Compatibility**: Verify compatibility with ACF
3. **Update SBOM**: Update [sbom.json](sbom.json) with new version information
4. **Document Changes**: Note breaking changes in release notes
5. **License Check**: Ensure license compatibility

## Communication

- **Issues**: Use GitHub Issues for bug reports and feature requests
- **Discussions**: Use GitHub Discussions for questions and ideas
- **Security**: Use private channels for security reports (see [SECURITY.md](SECURITY.md))
- **Code Review**: Use PR comments for code-related discussions

## Continuous Integration

- PRs trigger automated checks
- All checks must pass before merging
- Security scans are performed automatically
- Review and address any CI failures

### Auto-Fix on Build Failure

When a build fails on a pull request, our auto-fix system may automatically attempt to resolve common issues:

- **Missing Component Registration**: Automatically adds component typedefs and exports
- **Package Mismatches**: Updates PackageId in configuration files to match actual registrations
- **Common Configuration Issues**: Fixes obvious build configuration problems

**How it works:**
1. When a TeamCity CI build fails, the auto-fix workflow analyzes the error
2. If a common fixable pattern is detected, it applies the fix
3. A commit is automatically created and pushed to your PR branch
4. A comment is added to the PR explaining what was fixed
5. The build automatically re-runs

**What to do:**
- Review the auto-fix commit to ensure correctness
- If the fix is incorrect, you can revert it or make additional changes
- If build still fails, check the PR comment for manual fix suggestions

For more details, see [Auto-Fix Documentation](.github/workflows/AUTO_FIX_DOCUMENTATION.md).

## Recognition

Contributors are recognized in:

- Commit history
- Release notes (for significant contributions)
- Security advisories (for security researchers, with permission)

## Questions?

If you have questions about contributing:

- Review the [documentation](https://imagingtools.github.io/Acf/)
- Check existing issues and discussions
- Open a new issue for clarification
- Contact the maintainers

## Code of Conduct

We expect all contributors to:

- Be respectful and constructive
- Welcome newcomers
- Focus on what is best for the community
- Show empathy towards others

## Getting Help

Resources for contributors:

- **Documentation**: [https://imagingtools.github.io/Acf/](https://imagingtools.github.io/Acf/)
- **Component Framework Guide**: [Docs/ComponentFramework.md](Docs/ComponentFramework.md)
- **Security Policy**: [SECURITY.md](SECURITY.md)
- **CRA Compliance**: [CRA_COMPLIANCE.md](CRA_COMPLIANCE.md)
- **SBOM Guide**: [SBOM.md](SBOM.md)

## Thank You!

Your contributions help make ACF better and more secure for everyone. We appreciate your time and effort!

---

**Last Updated**: January 2026
