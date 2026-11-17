// Helpers of the TeamCity CI workflow for its analysis comment on a pull request.
// Load them in an actions/github-script step with require() after checking out .github/scripts.

const marker = '<!-- teamcity-ci-analysis -->';

// The comments that the workflow posted before it kept a marker start with this heading.
const legacyHeading = '## 🔍 TeamCity Build Failed - Error Analysis';

module.exports = {
  marker: marker,

  // Returns the analysis comments of the workflow in the pull request: the one with the marker and the older ones.
  async findComments(github, context) {
    const comments = await github.paginate(github.rest.issues.listComments, {
      owner: context.repo.owner,
      repo: context.repo.repo,
      issue_number: context.payload.pull_request.number
    });

    return comments.filter(c => c.user.type === 'Bot' && (c.body.startsWith(marker) || c.body.startsWith(legacyHeading)));
  },

  // Tells whether the run belongs to the latest commit of the pull request. A run of an older commit can still be
  // finishing after a newer one has started, and it must not change the comment of the newer one.
  async isCurrentHead(github, context) {
    const { data: pullRequest } = await github.rest.pulls.get({
      owner: context.repo.owner,
      repo: context.repo.repo,
      pull_number: context.payload.pull_request.number
    });

    return pullRequest.head.sha === context.payload.pull_request.head.sha;
  },

  // Collapses a comment as outdated or shows it again. A refusal of GitHub is a warning, it does not fail the run.
  async setMinimized(github, core, comment, minimized) {
    try {
      const state = await github.graphql(
        'query($id: ID!) { node(id: $id) { ... on IssueComment { isMinimized } } }',
        { id: comment.node_id }
      );
      if (state.node.isMinimized === minimized) {
        return;
      }

      if (minimized) {
        await github.graphql(
          'mutation($id: ID!) { minimizeComment(input: { subjectId: $id, classifier: OUTDATED }) { clientMutationId } }',
          { id: comment.node_id }
        );
      } else {
        await github.graphql(
          'mutation($id: ID!) { unminimizeComment(input: { subjectId: $id }) { clientMutationId } }',
          { id: comment.node_id }
        );
      }
    } catch (error) {
      core.warning(`Could not change the state of comment ${comment.id}: ${error.message}`);
    }
  }
};
