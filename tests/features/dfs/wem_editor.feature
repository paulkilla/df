@dfs_wem
Feature: WEM Demo Editor

  Scenario: Confirm the content curator editor workflow
    Given I am on "/user"
    When I fill in "edit-name" with "EricaWebb"
    And I fill in "edit-pass" with "password"
    And I press "Log in"
    Then I should see "EricaWebb"
    And I should see "Persona: Editor (Curator)"
    Given I am on the homepage
    Then I should see "Big Air at Big Sky" 
    When I follow "Big Air at Big Sky"
    Then I should see "Light powdery snow and consistently wide runs."
    When I follow "New draft"
    Then I should see "Big Air at Big Sky"
    When I follow "Edit draft"
    Then I should see "Edit WEM News Big Air at Big Sky"
    And I fill in "edit-title" with "Big Air at Big Sky, Montana"
    And I press "Send for Review"
    Then I should see "Big Air at Big Sky, Montana"
    And I should see "WEM News Big Air at Big Sky, Montana has been updated."
    When I follow "View draft"
    Then I should not see "WEM News Big Air at Big Sky, Montana has been updated."
    When I follow "Moderate"
    Then I should see "The current state is Needs Review."
    When I follow "Compare revisions"
    Then I should see "Edited by EricaWebb."
    When I press "Compare"
    Then I should see "Changes to Title"
    When I follow "View published"
    Then I should not see "Big Air at Big Sky, Montana"
    And I should see "Big Air at Big Sky"

  Scenario: Confirm the content reviewer editor workflow
    Given I am on "/user"
    When I fill in "edit-name" with "PaulSimon"
    And I fill in "edit-pass" with "password"
    And I press "Log in"
    Then I should see "PaulSimon"
    And I should see "Persona: Editor (Reviewer)"
    Given I am on "admin/workbench"
    Then I should see "My Profile"
    And I should see "PaulSimon"
    And I should see "My Edits"
    And I should see "All Recent Content"
    When I follow "Needs review"
    Then I should see "Big Air at Big Sky"
    When I follow "Big Air at Big Sky"
    Then I should see "Big Air at Big Sky"
    When I follow "View draft"
    Then I should see "Big Air at Big Sky, Montana"
    When I follow "Moderate"
    Then I select "Published" from "edit-state"
    When I press "Save"
    Then I should see "From Needs Review --> Published"
    And I should see "New draft"
    And I should not see "Edit draft"
    When I follow "View published"
    Then I should see "Big Air at Big Sky, Montana"
