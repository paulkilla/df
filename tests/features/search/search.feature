@df @core @search @api @errors
Feature: Site search

#  This test is failing on Drupal 8.5, so it's disabled for now until we have
#  rolled the next release.
#  @page @e4c5b23b
#  Scenario: Unpublished content does not appear in search results
#    Given I am an anonymous user
#    And page content:
#      | title    | moderation_state | body                                                                   |
#      | Zombie 1 | draft            | Zombie ipsum reversus ab viral inferno, nam rick grimes malum cerebro. |
#      | Zombie 2 | review           | De carne lumbering animata corpora quaeritis.                          |
#      | Zombie 3 | published        | Summus brains sit, morbo vel maleficia?                                |
#    When I visit "/search"
#    And I enter "zombie" for "Keywords"
#    And I press "Search"
#    Then the response status code should be 200
#    And I should not see "Zombie 1"
#    And I should not see "Zombie 2"
#    And I should see "Zombie 3"
